use rocket::serde::{Deserialize, json::Json};
use rocket_db_pools::Connection;

use crate::{db::gallery_db::GalleryDb, sql_exec_map_err};
use crate::db::schema::{ADD_IMAGE_TAG, ADD_TAG, DELETE_IMAGE_TAGS, QUERY_IMAGE_FOR_TAGS};

#[derive(Deserialize)]
pub struct TagUpdate {
    tags: Vec<String>,
}

#[patch("/<uuid>/tags", format = "json", data = "<form>")]
pub async fn edit_tags(mut db: Connection<GalleryDb>, uuid: &str, form: Json<TagUpdate>) -> Result<String, String> {
    sql_exec_map_err!(
        db,
        DELETE_IMAGE_TAGS,
        "Failed to clear old tags",
        uuid
    )?;

    for tag_value in &form.tags {
        // Ensure the tag exists in the 'tags' table
        sql_exec_map_err!(
            db,
            ADD_TAG,
            "Failed to ensure tag exists",
            tag_value
        )?;

        // Link this image to that tag in 'image_tags'
        sql_exec_map_err!(
            db,
            ADD_IMAGE_TAG,
            "Failed to link tag to image",
            uuid,
            tag_value
        )?;
    }

    Ok(format!("Successfully updated tags for {}", uuid))
}

#[get("/<uuid>/tags")]
pub async fn get_tags(mut db: Connection<GalleryDb>, uuid: &str) -> Result<Json<Vec<String>>, String> {
    let result: (Option<String>,) = sqlx::query_as(QUERY_IMAGE_FOR_TAGS)
        .bind(uuid)
        .fetch_one(&mut **db)
        .await
        .map_err(|e| format!("Failed to query tags: {}", e))?;

    let tags_vec = result.0
        .map(|s| s.split(',').map(|tag| tag.to_string()).collect())
        .unwrap_or_default();

    Ok(Json(tags_vec))
}
