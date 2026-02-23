/*
* INITIALIZATION
*/
const INIT_DB_TAGS: &str = r#"
    CREATE TABLE IF NOT EXISTS tags (
        id    INTEGER PRIMARY KEY AUTOINCREMENT,
        value TEXT UNIQUE NOT NULL
    );
"#;

const INIT_DB_IMAGES: &str = r#"
    CREATE TABLE IF NOT EXISTS images (
        id         INTEGER PRIMARY KEY AUTOINCREMENT,
        uuid       TEXT UNIQUE NOT NULL,
        date_added DATETIME NOT NULL DEFAULT (datetime('now', 'localtime'))
    );
"#;

const INIT_DB_IMAGE_TAGS: &str = r#"
    CREATE TABLE IF NOT EXISTS image_tags (
        image_id INTEGER,
        tag_id   INTEGER,
        PRIMARY KEY (image_id, tag_id),
        FOREIGN KEY (image_id) REFERENCES images(id) ON DELETE CASCADE,
        FOREIGN KEY (tag_id)   REFERENCES tags(id)   ON DELETE CASCADE
    );
"#;


/*
* DROPPING
*/
const DROP_DB_IMAGES: &str = r#"
    DROP TABLE IF EXISTS images;
"#;

const DROP_DB_TAGS: &str = r#"
    DROP TABLE IF EXISTS tags;
"#;

const DROP_DB_IMAGE_TAGS: &str = r#"
    DROP TABLE IF EXISTS image_tags;
"#;

const DROP_MIGRATIONS: &str = r#"
    DROP TABLE IF EXISTS _sql_migrations;
"#;


/*
* INSERTING
*/
const ADD_IMAGE: &str = r#"
    INSERT INTO images (uuid)
    VALUES ($1);
"#;

const ADD_TAG: &str = r#"
    INSERT OR IGNORE INTO tags (value)
    VALUES ($1);
"#;

const ADD_IMAGE_TAG: &str = r#"
    INSERT INTO image_tags (image_id, tag_id)
    VALUES (
        (SELECT id FROM images WHERE uuid  = $1),
        (SELECT id FROM tags   WHERE value = $2)
    );
"#;


/*
* QUERYING
*/
const QUERY_IMAGE_BY_TAGS: &str = r#"
    SELECT i.uuid
    FROM images     i
    JOIN image_tags it ON i.id = it.image_id
    JOIN tags       t  ON t.id = it.tag_id
    WHERE t.value IN ({})
    GROUP BY i.id
    HAVING COUNT(DISTINCT t.value) = $1;
"#;

const QUERY_IMAGE_FOR_TAGS: &str = r#"
    SELECT GROUP_CONCAT(t.value) as tags
    FROM tags t
    JOIN image_tags it ON t.id = it.tag_id
    JOIN images i ON i.id = it.image_id
    WHERE i.uuid = $1;
"#;


/*
* DELETING
*/
const REMOVE_IMAGE: &str = r#"
    DELETE FROM images WHERE uuid = $1;
"#;

const DELETE_IMAGE_TAGS: &str = r#"
    DELETE FROM image_tags
    WHERE image_id = (SELECT id FROM images WHERE uuid = $1);
"#;


use sqlx::{SqliteConnection, Row};
use super::models::DbQueryImageUUID;

pub async fn init_db(conn: &mut SqliteConnection) -> Result<(), sqlx::Error> {
    sqlx::query(INIT_DB_IMAGES).execute(&mut *conn).await?;
    sqlx::query(INIT_DB_TAGS).execute(&mut *conn).await?;
    sqlx::query(INIT_DB_IMAGE_TAGS).execute(&mut *conn).await?;
    Ok(())
}

pub async fn nuke_db(conn: &mut SqliteConnection) -> Result<(), sqlx::Error> {
    sqlx::query(DROP_DB_IMAGE_TAGS).execute(&mut *conn).await?;
    sqlx::query(DROP_DB_TAGS).execute(&mut *conn).await?;
    sqlx::query(DROP_DB_IMAGES).execute(&mut *conn).await?;
    sqlx::query(DROP_MIGRATIONS).execute(&mut *conn).await?;
    Ok(())
}

// --- INSERTION LOGIC ---

pub async fn add_image(conn: &mut SqliteConnection, uuid: &str) -> Result<(), sqlx::Error> {
    sqlx::query(ADD_IMAGE)
        .bind(uuid)
        .execute(conn)
        .await?;
    Ok(())
}

pub async fn add_tag(conn: &mut SqliteConnection, value: &str) -> Result<(), sqlx::Error> {
    sqlx::query(ADD_TAG)
        .bind(value)
        .execute(conn)
        .await?;
    Ok(())
}

/// Links an existing image (by uuid) to an existing tag (by value)
pub async fn link_image_tag(
    conn: &mut SqliteConnection,
    uuid: &str,
    tag: &str
) -> Result<(), sqlx::Error> {
    sqlx::query(ADD_IMAGE_TAG)
        .bind(uuid)
        .bind(tag)
        .execute(conn)
        .await?;
    Ok(())
}

// --- QUERYING LOGIC ---

/// Returns UUIDs of images that have ALL the provided tags
pub async fn get_images_by_tags(
    conn: &mut SqliteConnection,
    tags: Vec<String>
) -> Result<Vec<DbQueryImageUUID>, sqlx::Error> {
    if tags.is_empty() { return Ok(vec![]); }

    let count = tags.len();
    // Create placeholders: $1, $2, $3...
    let placeholders = (1..=count)
        .map(|i| format!("${}", i))
        .collect::<Vec<_>>()
        .join(",");

    // Replace the {} in your SQL constant
    let query_str = QUERY_IMAGE_BY_TAGS.replace("{}", &placeholders);

    // Use query_as to map rows directly to your struct
    let mut query = sqlx::query_as::<_, DbQueryImageUUID>(&query_str);

    // Bind all the tags
    for tag in &tags {
        query = query.bind(tag);
    }

    // Bind the final $n placeholder for the HAVING clause
    // This will be $(count + 1)
    query = query.bind(count as i32);

    query.fetch_all(conn).await
}

/// Returns a comma-separated string of tags for a specific image uuid
pub async fn get_tags_for_image(
    conn: &mut SqliteConnection,
    uuid: &str
) -> Result<Option<String>, sqlx::Error> {
    let row = sqlx::query(QUERY_IMAGE_FOR_TAGS)
        .bind(uuid)
        .fetch_optional(conn)
        .await?;

    Ok(row.map(|r| r.get("tags")))
}

// --- DELETION LOGIC ---

pub async fn remove_image(conn: &mut SqliteConnection, uuid: &str) -> Result<(), sqlx::Error> {
    // Note: Since you have ON DELETE CASCADE on your foreign keys,
    // deleting from 'images' will automatically clear 'image_tags'.
    sqlx::query(REMOVE_IMAGE)
        .bind(uuid)
        .execute(conn)
        .await?;
    Ok(())
}

pub async fn remove_image_tags(conn: &mut SqliteConnection, uuid: &str) -> Result<(), sqlx::Error> {
    // Note: Since you have ON DELETE CASCADE on your foreign keys,
    // deleting from 'images' will automatically clear 'image_tags'.
    sqlx::query(DELETE_IMAGE_TAGS)
        .bind(uuid)
        .execute(conn)
        .await?;
    Ok(())
}
