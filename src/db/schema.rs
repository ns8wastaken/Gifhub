pub const INIT_DB_TAGS: &str = r#"
    CREATE TABLE IF NOT EXISTS tags (
        id    INTEGER PRIMARY KEY AUTOINCREMENT,
        value TEXT UNIQUE NOT NULL
    );
"#;

pub const INIT_DB_IMAGES: &str = r#"
    CREATE TABLE IF NOT EXISTS images (
        id         INTEGER PRIMARY KEY AUTOINCREMENT,
        uuid       TEXT UNIQUE NOT NULL,
        date_added DATETIME NOT NULL DEFAULT (datetime('now', 'localtime'))
    );
"#;

pub const INIT_DB_IMAGE_TAGS: &str = r#"
    CREATE TABLE IF NOT EXISTS image_tags (
        image_id INTEGER,
        tag_id INTEGER,
        PRIMARY KEY (image_id, tag_id),
        FOREIGN KEY (image_id) REFERENCES images(id) ON DELETE CASCADE,
        FOREIGN KEY (tag_id)   REFERENCES tags(id)   ON DELETE CASCADE
    );
"#;


pub const DROP_DB_IMAGES: &str = r#"
    DROP TABLE IF EXISTS images;
"#;

pub const DROP_DB_TAGS: &str = r#"
    DROP TABLE IF EXISTS tags;
"#;

pub const DROP_DB_IMAGE_TAGS: &str = r#"
    DROP TABLE IF EXISTS image_tags;
"#;


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


pub const QUERY_IMAGE_BY_TAGS: &str = r#"
    SELECT i.uuid
    FROM images     i
    JOIN image_tags it ON i.id = it.image_id
    JOIN tags       t  ON t.id = it.tag_id
    WHERE t.value IN ({})
    GROUP BY i.id
    HAVING COUNT(DISTINCT t.value) = $1;
"#;
