use rocket_db_pools::Connection;
use rocket::State;
use sqlx::Acquire;

use crate::errors::ApiError;
use crate::GifhubDb;
use crate::db::repository::Repository;
use crate::AppConfig;

#[post("/nuke-files")]
pub async fn nuke(
    config: &State<AppConfig>,
    mut conn: Connection<GifhubDb>,
    pool: &State<GifhubDb>
) -> Result<String, ApiError> {
    let mut tx = conn.begin().await?;
    Repository::new(&mut *tx).nuke().await?;
    tx.commit().await?;

    sqlx::migrate!("./migrations")
        .run(&***pool)
        .await?;

    std::fs::remove_dir_all(&config.gallery_path)?;
    std::fs::create_dir(&config.gallery_path)?;

    Ok(String::from("Successfully reset the gallery"))
}
