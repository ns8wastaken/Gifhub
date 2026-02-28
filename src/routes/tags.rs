use rocket::serde::{Deserialize, json::Json};
use rocket_db_pools::Connection;
use sqlx::Acquire;

use crate::db::gifhub_db::GifhubDb;
use crate::db::repository::Repository;
use crate::errors::ApiError;

// TODO: just use a string and stop being stupid
#[derive(Deserialize)]
pub struct TagUpdate {
    tags: Vec<String>,
}

#[patch("/<uuid>/tags", format = "json", data = "<form>")]
pub async fn edit_tags(
    mut conn: Connection<GifhubDb>,
    uuid: &str, form: Json<TagUpdate>
) -> Result<String, ApiError> {
    let mut tx = conn.begin().await?;

    let mut repo = Repository::new(&mut *tx);

    repo.remove_image_tags(uuid).await?;

    for tag_value in &form.tags {
        // Ensure the tag exists in the 'tags' table
        repo.add_tag(tag_value).await?;

        // Link this image to that tag in 'image_tags'
        repo.link_image_tag(uuid, tag_value).await?
    }

    tx.commit().await?;

    Ok(format!("Successfully updated tags for {}", uuid))
}

#[get("/<uuid>/tags")]
pub async fn get_tags(mut conn: Connection<GifhubDb>, uuid: &str) -> Result<Json<Vec<String>>, ApiError> {
    let result = Repository::new(&mut conn)
        .get_tags_for_image(uuid)
        .await?;

    let tags_vec = result
        .map(|s| s.split(',').map(|tag| tag.to_string()).collect())
        .unwrap_or_default();

    Ok(Json(tags_vec))
}
