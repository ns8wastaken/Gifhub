use rocket::serde::json::Json;
use rocket::State;
use rocket_db_pools::Connection;

use crate::db::models::DbQueryImageUUID;
use crate::errors::ApiError;
use crate::{AppConfig, GifhubDb};
use crate::db::repository::Repository;

#[get("/images")]
pub fn images(config: &State<AppConfig>) -> Json<Vec<String>> {
    let mut files: Vec<String> = Vec::new();

    if let Ok(entries) = std::fs::read_dir(&config.gallery_path) {
        for entry in entries.flatten() {
            if let Some(name) = entry.file_name().to_str() {
                files.push(name.to_string());
            }
        }
    }

    Json(files)
}

#[get("/search?<q>")]
pub async fn search_db(mut conn: Connection<GifhubDb>, q: String) -> Result<Json<Vec<DbQueryImageUUID>>, ApiError> {
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
