use rocket_db_pools::Connection;
use rocket::State;
use sqlx::Acquire;

use crate::{db::gallery_db::GalleryDb};
use crate::db::schema::{nuke_db, init_db};
use crate::AppConfig;

#[post("/nuke-files")]
pub async fn nuke(config: &State<AppConfig>, mut db: Connection<GalleryDb>) -> Result<String, String> {
    let mut tx = db
        .begin()
        .await
        .map_err(|e| format!("Could not begin transaction: {e}"))?;

    // Nuke db
    nuke_db(&mut *tx)
        .await
        .map_err(|e| format!("Failed to nuke db: {e}"))?;

    // Init db
    init_db(&mut *tx)
        .await
        .map_err(|e| format!("Failed to recreate tables: {e}"))?;

    tx
        .commit()
        .await
        .map_err(|e| format!("Could not commit transaction: {e}"))?;

    // Remove all images
    std::fs::remove_dir_all(&config.gallery_path)
        .map_err(|e| format!("Failed to remove gallery directory: {e}"))?;
    std::fs::create_dir(&config.gallery_path)
        .map_err(|e| format!("Failed to recreate gallery directory: {e}"))?;

    Ok(String::from("Successfully reset the gallery"))
}
