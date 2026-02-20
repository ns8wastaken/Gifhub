use rocket_db_pools::Connection;

use crate::{db::gallery_db::GalleryDb, sql_exec_map_err};
use crate::db::schema::REMOVE_IMAGE;

#[delete("/<uuid>")]
pub async fn delete_image(mut db: Connection<GalleryDb>, uuid: &str) -> Result<String, String> {
    // Delete from image table
    // TODO: delete from other tables
    sql_exec_map_err!(
        db,
        REMOVE_IMAGE,
        "Failed to delete image from the db",
        uuid
    )?;

    std::fs::remove_file(format!("gallery/{}", uuid))
        .map_err(|e| format!("Failed to delete image from the gallery: {}", e))?;

    Ok(String::from("Successfully deleted the image"))
}
