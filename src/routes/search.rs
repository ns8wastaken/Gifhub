use rocket::serde::json::Json;
use rocket_db_pools::Connection;
use dyn_fmt::AsStrFormatExt;

use crate::GalleryDb;
use crate::db::{schema::QUERY_IMAGE_BY_TAGS, models::DbQueryImage};

#[get("/images")]
pub fn images() -> Json<Vec<String>> {
    let image_dir = "gallery/";
    let mut files: Vec<String> = Vec::new();

    if let Ok(entries) = std::fs::read_dir(image_dir) {
        for entry in entries.flatten() {
            if let Some(name) = entry.file_name().to_str() {
                files.push(name.to_string());
            }
        }
    }

    Json(files)
}

#[get("/search?<q>")]
pub async fn search_db(mut db: Connection<GalleryDb>, q: String) -> Result<Json<Vec<DbQueryImage>>, String> {
    let tags = q
        .split(',')
        .filter_map(|t| {
            let t = t.trim();
            (!t.is_empty()).then(|| {
                format!("'{}'", t)
            })
        })
        .collect::<Vec<String>>();

    let count = tags.len() as i64;
    let query = QUERY_IMAGE_BY_TAGS.format([&tags.join(",")]);

    let results: Vec<DbQueryImage> = sqlx::query_as(&query)
        .bind(count)
        .fetch_all(&mut **db)
        .await
        .map_err(|e| format!("Failed to query database: {}", e))?;

    Ok(Json(results))
}
