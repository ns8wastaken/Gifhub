use sqlx::{SqliteConnection, Row};

use super::error::DbError;
use super::models::DbQueryImageUUID;
use super::schema::*;

pub struct Repository<'a> {
    conn: &'a mut SqliteConnection,
}

impl<'a> Repository<'a> {
    pub fn new(conn: &'a mut SqliteConnection) -> Self {
        Self { conn }
    }

    pub async fn nuke(&mut self) -> Result<(), DbError> {
        sqlx::query(DROP_DB_IMAGE_TAGS).execute(&mut *self.conn).await?;
        sqlx::query(DROP_DB_TAGS).execute(&mut *self.conn).await?;
        sqlx::query(DROP_DB_IMAGES).execute(&mut *self.conn).await?;
        sqlx::query(DROP_MIGRATIONS).execute(&mut *self.conn).await?;
        Ok(())
    }

    pub async fn add_image(&mut self, uuid: &str) -> Result<(), DbError> {
        sqlx::query(ADD_IMAGE)
            .bind(uuid)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }

    pub async fn add_tag(&mut self, value: &str) -> Result<(), DbError> {
        sqlx::query(ADD_TAG)
            .bind(value)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }

    /// Links an existing image (by uuid) to an existing tag (by value)
    pub async fn link_image_tag(&mut self, uuid: &str, tag: &str) -> Result<(), DbError> {
        sqlx::query(ADD_IMAGE_TAG)
            .bind(uuid)
            .bind(tag)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }

    /// Returns UUIDs of images that have ALL the provided tags
    pub async fn get_images_by_tags(
        &mut self, tags: Vec<String>
    ) -> Result<Vec<DbQueryImageUUID>, DbError> {
        if tags.is_empty() { return Ok(vec![]); }

        let count = tags.len();

        let placeholders = (1..=count)
            .map(|i| format!("${}", i + 1))
            .collect::<Vec<_>>()
            .join(",");

        let query_str = QUERY_IMAGE_BY_TAGS.replace("{}", &placeholders);

        let mut query = sqlx::query_as::<_, DbQueryImageUUID>(&query_str);

        query = query.bind(count as i32);

        for tag in &tags {
            query = query.bind(tag);
        }

        Ok(query.fetch_all(&mut *self.conn).await?)
    }

    /// Returns a comma-separated string of tags for a specific image uuid
    pub async fn get_tags_for_image(
        &mut self,
        uuid: &str
    ) -> Result<Option<String>, DbError> {
        let row = sqlx::query(QUERY_IMAGE_FOR_TAGS)
            .bind(uuid)
            .fetch_optional(&mut *self.conn)
            .await?;

        Ok(row.map(|r| r.get("tags")))
    }

    pub async fn remove_image(&mut self, uuid: &str) -> Result<(), DbError> {
        // NOTE: Using ON DELETE CASCADE makes this delete from `image_tags` as well
        sqlx::query(REMOVE_IMAGE)
            .bind(uuid)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }

    pub async fn remove_image_tags(&mut self, uuid: &str) -> Result<(), DbError> {
        // NOTE: Using ON DELETE CASCADE makes this delete from `image_tags` as well
        sqlx::query(DELETE_IMAGE_TAGS)
            .bind(uuid)
            .execute(&mut *self.conn)
            .await?;
        Ok(())
    }
}
