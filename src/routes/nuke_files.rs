use rocket_db_pools::Connection;
use rocket::State;

use crate::{db::gallery_db::GalleryDb};
use crate::db::schema::{nuke_db, init_db};
use crate::AppConfig;

#[post("/nuke-files")]
pub async fn nuke(config: &State<AppConfig>, mut db: Connection<GalleryDb>) -> Result<String, String> {
    // Nuke db
    nuke_db(&mut **db)
        .await
        .map_err(|e| format!("Failed to nuke db: {e}"))?;

    // Init db
    init_db(&mut **db)
        .await
        .map_err(|e| format!("Failed to recreate tables: {e}"))?;

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
