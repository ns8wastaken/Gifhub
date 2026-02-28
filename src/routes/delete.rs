use rocket_db_pools::Connection;
use rocket::State;

use crate::db::gifhub_db::GifhubDb;
use crate::db::repository::Repository;
use crate::errors::ApiError;
use crate::AppConfig;

#[delete("/<uuid>")]
pub async fn delete_image(
    config: &State<AppConfig>,
    mut conn: Connection<GifhubDb>,
    uuid: &str
) -> Result<String, ApiError> {
    Repository::new(&mut conn)
        .remove_image(uuid)
        .await?;

    let path = config.gallery_path.join(&uuid);
    std::fs::remove_file(path)?;

    Ok(String::from("Successfully deleted the image"))
}
