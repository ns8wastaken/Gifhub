use rocket_db_pools::Connection;
use rocket::State;

use crate::{db::gallery_db::GalleryDb, sql_exec_map_err};
use crate::db::schema::*;
use crate::AppConfig;

#[post("/nuke-files")]
pub async fn nuke(config: &State<AppConfig>, mut db: Connection<GalleryDb>) -> Result<String, String> {
    // Nuke db
    sql_exec_map_err!(db, DROP_DB_IMAGE_TAGS, "Failed to drop 'image_tags'")?;
    sql_exec_map_err!(db, DROP_DB_IMAGES,     "Failed to drop 'images'")?;
    sql_exec_map_err!(db, DROP_DB_TAGS,       "Failed to drop 'tags'")?;

    // Init db
    sql_exec_map_err!(db, INIT_DB_TAGS,       "Failed to create 'tags'")?;
    sql_exec_map_err!(db, INIT_DB_IMAGES,     "Failed to create 'images'")?;
    sql_exec_map_err!(db, INIT_DB_IMAGE_TAGS, "Failed to create 'image_tags'")?;

    // Remove all images
    let iter = std::fs::read_dir(&config.gallery_path)
        .map_err(|e| format!(
            "Failed to read '{}' directory: {}",
            config.gallery_path.to_string_lossy(),
            e
        ))?;

    for entry in iter {
        let path = entry
            .map_err(|e| format!("Failed to access directory entry: {}", e))?
            .path();

        std::fs::remove_file(&path)
            .map_err(|e| format!(
                "Insufficient permissions to delete '{}': {}",
                path.to_string_lossy(),
                e
            ))?;
    }

    Ok(String::from("Successfully reset the gallery"))
}
