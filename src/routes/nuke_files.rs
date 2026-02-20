use rocket_db_pools::Connection;

use crate::{db::gallery_db::GalleryDb, sql_exec_map_err};
use crate::db::schema::*;

#[post("/nuke-files")]
pub async fn nuke(mut db: Connection<GalleryDb>) -> Result<String, String> {
    let gallery_path = std::path::Path::new("gallery");

    // Clear gallery.sqlite
    sql_exec_map_err!(db, DROP_DB_IMAGE_TAGS, "Failed to drop 'image_tags'")?;
    sql_exec_map_err!(db, DROP_DB_IMAGES,     "Failed to drop 'images'")?;
    sql_exec_map_err!(db, DROP_DB_TAGS,       "Failed to drop 'tags'")?;

    // Init gallery.sqlite
    sql_exec_map_err!(db, INIT_DB_TAGS,       "Failed to create 'tags'")?;
    sql_exec_map_err!(db, INIT_DB_IMAGES,     "Failed to create 'images'")?;
    sql_exec_map_err!(db, INIT_DB_IMAGE_TAGS, "Failed to create 'image_tags'")?;

    // Remove all images
    let iter = std::fs::read_dir(gallery_path)
        .map_err(|e| format!("Failed to read 'gallery/' directory: {}", e))?;

    for entry in iter {
        let path = entry
            .map_err(|e| format!("Failed to access directory entry: {}", e))?
            .path();

        std::fs::remove_file(&path)
            .map_err(|e| {
                format!(
                    "Insufficient permissions to delete '{}': {}",
                    path.to_string_lossy(),
                    e
                )
            })?;
    }

    Ok(String::from("Successfully reset the gallery"))
}
