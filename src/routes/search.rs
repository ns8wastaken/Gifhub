use rocket::serde::json::Json;
use rocket::State;
use rocket_db_pools::Connection;
use dyn_fmt::AsStrFormatExt;

use crate::{AppConfig, GalleryDb};
use crate::db::{schema::QUERY_IMAGE_BY_TAGS, models::DbQueryImageUUID};

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
pub async fn search_db(mut db: Connection<GalleryDb>, q: String) -> Result<Json<Vec<DbQueryImageUUID>>, String> {
    let tags: Vec<String> = q
        .split(',')
        .filter_map(|t| {
            let t = t.trim();
            (!t.is_empty()).then(|| format!("'{}'", t))
        })
        .collect();

    let count = tags.len() as i64;
    let query = QUERY_IMAGE_BY_TAGS.format([&tags.join(",")]);

    let results: Vec<DbQueryImageUUID> = sqlx::query_as(&query)
        .bind(count)
        .fetch_all(&mut **db)
        .await
        .map_err(|e| format!("Failed to query database: {}", e))?;

    Ok(Json(results))
}
