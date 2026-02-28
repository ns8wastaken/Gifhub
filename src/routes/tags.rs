use rocket::serde::{Deserialize, json::Json};
use rocket_db_pools::Connection;
use sqlx::Acquire;

use crate::db::gallery_db::GalleryDb;
use crate::db::schema::{
    remove_image_tags,
    add_tag,
    link_image_tag,
    get_tags_for_image,
};

// TODO: just use a string and stop being stupid
#[derive(Deserialize)]
pub struct TagUpdate {
    tags: Vec<String>,
}

#[patch("/<uuid>/tags", format = "json", data = "<form>")]
pub async fn edit_tags(mut db: Connection<GalleryDb>, uuid: &str, form: Json<TagUpdate>) -> Result<String, String> {
    let mut tx = db
        .begin()
        .await
        .map_err(|e| format!("Could not begin transaction: {e}"))?;

    remove_image_tags(&mut *tx, uuid)
        .await
        .map_err(|e| format!("Failed to clear old tags: {e}"))?;

    for tag_value in &form.tags {
        // Ensure the tag exists in the 'tags' table
        add_tag(&mut *tx, tag_value)
            .await
            .map_err(|e| format!("Failed to ensure tag exists: {e}"))?;

        // Link this image to that tag in 'image_tags'
        link_image_tag(&mut *tx, uuid, tag_value)
            .await
            .map_err(|e| format!("Failed to link tag to image: {e}"))?;
    }

    tx
        .commit()
        .await
        .map_err(|e| format!("Could not commit transaction: {e}"))?;

    Ok(format!("Successfully updated tags for {}", uuid))
}

#[get("/<uuid>/tags")]
pub async fn get_tags(mut db: Connection<GalleryDb>, uuid: &str) -> Result<Json<Vec<String>>, String> {
    let result = get_tags_for_image(&mut **db, uuid)
        .await
        .map_err(|e| format!("Failed to query tags: {e}"))?;

    let tags_vec = result
        .map(|s| s.split(',').map(|tag| tag.to_string()).collect())
        .unwrap_or_default();

    Ok(Json(tags_vec))
}
