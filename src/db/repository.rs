use sqlx::{QueryBuilder, Row, SqliteConnection};


use super::error::DbError;
use super::models::{DbQueryImage, DbQueryImageUUID, TagItem};

pub struct Repository<'a> {
    conn: &'a mut SqliteConnection,
}

#[derive(sqlx::FromRow)]
struct ImageRow {
    uuid: String,
    tag: Option<String>,
}

impl<'a> Repository<'a> {
    pub fn new(conn: &'a mut SqliteConnection) -> Self {
        Self { conn }
    }

    pub async fn nuke(&mut self) -> Result<(), DbError> {
        sqlx::query("DROP TABLE IF EXISTS image_tags;").execute(&mut *self.conn).await?;
        sqlx::query("DROP TABLE IF EXISTS tags;").execute(&mut *self.conn).await?;
        sqlx::query("DROP TABLE IF EXISTS images;").execute(&mut *self.conn).await?;
        sqlx::query("DROP TABLE IF EXISTS _sqlx_migrations;").execute(&mut *self.conn).await?;
        Ok(())
    }

    pub async fn add_image(&mut self, uuid: &str) -> Result<(), DbError> {
        sqlx::query("INSERT INTO images (uuid) VALUES ($1);")
            .bind(uuid)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }

    pub async fn add_tag(&mut self, value: &str) -> Result<(), DbError> {
        sqlx::query("INSERT OR IGNORE INTO tags (value) VALUES ($1);")
            .bind(value)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }

    /// Links an existing image (by uuid) to an existing tag (by value)
    pub async fn link_image_tag(&mut self, uuid: &str, tag: &str) -> Result<(), DbError> {
        sqlx::query(
            r#"
            INSERT INTO image_tags (image_id, tag_id)
            VALUES (
                (SELECT id FROM images WHERE uuid  = $1),
                (SELECT id FROM tags   WHERE value = $2)
            );
            "#
        )
            .bind(uuid)
            .bind(tag)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }

    pub async fn get_all_tags(&mut self) -> Result<Vec<TagItem>, DbError> {
        let tags = sqlx::query_as(
            r#"
            SELECT t.value, COUNT(it.image_id) as count
            FROM tags t
            LEFT JOIN image_tags it ON t.id = it.tag_id
            GROUP BY t.id, t.value
            ORDER BY count DESC, t.value ASC
            "#
        )
            .fetch_all(&mut *self.conn)
            .await?;

        Ok(tags)
    }

    pub async fn get_all_images(&mut self) -> Result<Vec<DbQueryImage>, DbError> {
        let rows = sqlx::query_as::<_, ImageRow>(
            r#"
            SELECT i.uuid, t.value as tag
            FROM images i
            LEFT JOIN image_tags it ON i.id = it.image_id
            LEFT JOIN tags t ON it.tag_id = t.id
            ORDER BY i.uuid
            "#
        )
            .fetch_all(&mut *self.conn)
            .await?;

        let mut images: Vec<DbQueryImage> = Vec::new();

        for row in rows {
            if let Some(last) = images.last_mut() {
                if last.uuid == row.uuid {
                    if let Some(tag) = row.tag {
                        last.tags.push(tag);
                    }
                    continue;
                }
            }
            images.push(DbQueryImage {
                uuid: row.uuid,
                tags: row.tag.into_iter().collect(),
            });
        }

        Ok(images)
    }

    /// Returns UUIDs of images that have ALL the provided tags
    pub async fn get_images_by_tags(
        &mut self,
        tags: Vec<String>
    ) -> Result<Vec<DbQueryImageUUID>, DbError> {
        if tags.is_empty() { return Ok(vec![]); }

        let mut qbuilder = QueryBuilder::new(
            r#"
             SELECT i.uuid
            FROM images     i
            JOIN image_tags it ON i.id = it.image_id
            JOIN tags       t  ON t.id = it.tag_id
            WHERE t.value IN (
            "#
        );

        let mut sep = qbuilder.separated(",");
        for tag in &tags {
            sep.push_bind(tag);
        }

        qbuilder.push(") GROUP BY i.id HAVING COUNT(DISTINCT t.value) =");
        qbuilder.push_bind(tags.len() as i32);

        Ok(qbuilder.build_query_as().fetch_all(&mut *self.conn).await?)
    }

    /// Returns a comma-separated string of tags for a specific image uuid
    pub async fn get_tags_for_image(
        &mut self,
        uuid: &str
    ) -> Result<Option<String>, DbError> {
        let row = sqlx::query(
            r#"
            SELECT GROUP_CONCAT(t.value) as tags
            FROM tags t
            JOIN image_tags it ON t.id = it.tag_id
            JOIN images i ON i.id = it.image_id
            WHERE i.uuid = $1;
            "#
        )
            .bind(uuid)
            .fetch_optional(&mut *self.conn)
            .await?;

        Ok(row.map(|r| r.get("tags")))
    }

    pub async fn remove_image(&mut self, uuid: &str) -> Result<(), DbError> {
        // NOTE: Using ON DELETE CASCADE makes this delete from `image_tags` as well
        sqlx::query("DELETE FROM images WHERE uuid = $1;")
            .bind(uuid)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }

    pub async fn remove_image_tags(&mut self, uuid: &str) -> Result<(), DbError> {
        // NOTE: Using ON DELETE CASCADE makes this delete from `image_tags` as well
        sqlx::query(
            r#"
             DELETE FROM image_tags
            WHERE image_id = (SELECT id FROM images WHERE uuid = $1);
            "#
        )
            .bind(uuid)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }
}
