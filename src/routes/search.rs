use rocket::serde::json::Json;
use rocket_db_pools::Connection;

use crate::db::models::{DbQueryImage, DbQueryImageUUID};
use crate::errors::ApiError;
use crate::GifhubDb;
use crate::db::repository::Repository;

#[get("/images")]
pub async fn images(mut conn: Connection<GifhubDb>) -> Result<Json<Vec<DbQueryImage>>, ApiError> {
    let images = Repository::new(&mut conn)
        .get_all_images()
        .await?;

    Ok(Json(images))
}

#[get("/search?<q>")]
pub async fn search_db(
    mut conn: Connection<GifhubDb>,
    q: String
) -> Result<Json<Vec<DbQueryImageUUID>>, ApiError> {
    let tags: Vec<String> = q
        .split(',')
        .filter_map(|t| {
            let t = t.trim();
            (!t.is_empty()).then(|| format!("{}", t))
        })
        .collect();

    let res = Repository::new(&mut conn)
        .get_images_by_tags(tags)
        .await?;

    Ok(Json(res))
}
