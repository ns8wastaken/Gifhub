use rocket_db_pools::Connection;
use rocket::State;

use crate::{db::gallery_db::GalleryDb, sql_exec_map_err};
use crate::db::schema::REMOVE_IMAGE;
use crate::AppConfig;

#[delete("/<uuid>")]
pub async fn delete_image(config: &State<AppConfig>, mut db: Connection<GalleryDb>, uuid: &str) -> Result<String, String> {
    // Delete from image table
    // TODO: delete from other tables
    sql_exec_map_err!(
        db,
        REMOVE_IMAGE,
        "Failed to delete image from the db",
        uuid
    )?;

    let path = config.gallery_path.join(&uuid);
    std::fs::remove_file(path)
        .map_err(|e| format!("Failed to delete image from the gallery: {}", e))?;

    Ok(String::from("Successfully deleted the image"))
}
