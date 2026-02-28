use rocket::State;
use rocket::form::Form;
use rocket::fs::TempFile;
use rocket_db_pools::Connection;
use sqlx::Acquire;
use uuid::Uuid;

use crate::errors::ApiError;
use crate::{AppConfig, GifhubDb};
use crate::db::repository::Repository;

#[derive(FromForm)]
pub struct UploadForm<'a> {
    pub file: TempFile<'a>,
    pub tags: String,
}

#[post("/upload", data = "<form>")]
pub async fn file(
    config: &State<AppConfig>,
    mut form: Form<UploadForm<'_>>,
    mut conn: Connection<GifhubDb>
) -> Result<String, ApiError> {
    let uuid = Uuid::new_v4().to_string();
    let new_filepath = config.gallery_path.join(&uuid);

    if let Err(e) = form.file.copy_to(&new_filepath).await {
        return Err(ApiError::FileWrite(e));
    }

    let mut tx = conn.begin().await?;

    let mut repo = Repository::new(&mut *tx);

    repo.add_image(&uuid).await?;

    for tag in form.tags.split(',') {
        let tag = tag.trim().to_lowercase();

        repo.add_tag(&tag).await?;

        repo.link_image_tag(&uuid, &tag).await?;
    }

    tx.commit().await?;

    Ok(String::from("Successfully uploaded images and tags"))
}
