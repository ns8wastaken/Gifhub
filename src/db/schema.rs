/*
* DROPPING
*/
pub const DROP_DB_IMAGES: &str = r#"
    DROP TABLE IF EXISTS images;
"#;

pub const DROP_DB_TAGS: &str = r#"
    DROP TABLE IF EXISTS tags;
"#;

pub const DROP_DB_IMAGE_TAGS: &str = r#"
    DROP TABLE IF EXISTS image_tags;
"#;

pub const DROP_MIGRATIONS: &str = r#"
    DROP TABLE IF EXISTS _sql_migrations;
"#;


/*
* INSERTING
*/
pub const ADD_IMAGE: &str = r#"
    INSERT INTO images (uuid)
    VALUES ($1);
"#;

pub const ADD_TAG: &str = r#"
    INSERT OR IGNORE INTO tags (value)
    VALUES ($1);
"#;

pub const ADD_IMAGE_TAG: &str = r#"
    INSERT INTO image_tags (image_id, tag_id)
    VALUES (
        (SELECT id FROM images WHERE uuid  = $1),
        (SELECT id FROM tags   WHERE value = $2)
    );
"#;


/*
* QUERYING
*/
pub const QUERY_IMAGE_BY_TAGS: &str = r#"
    SELECT i.uuid
    FROM images     i
    JOIN image_tags it ON i.id = it.image_id
    JOIN tags       t  ON t.id = it.tag_id
    WHERE t.value IN ({})
    GROUP BY i.id
    HAVING COUNT(DISTINCT t.value) = $1;
"#;

pub const QUERY_IMAGE_FOR_TAGS: &str = r#"
    SELECT GROUP_CONCAT(t.value) as tags
    FROM tags t
    JOIN image_tags it ON t.id = it.tag_id
    JOIN images i ON i.id = it.image_id
    WHERE i.uuid = $1;
"#;


/*
* DELETING
*/
pub const REMOVE_IMAGE: &str = r#"
    DELETE FROM images WHERE uuid = $1;
"#;

pub const DELETE_IMAGE_TAGS: &str = r#"
    DELETE FROM image_tags
    WHERE image_id = (SELECT id FROM images WHERE uuid = $1);
"#;
