use rocket::State;
use rocket::form::Form;
use rocket::fs::TempFile;
use rocket_db_pools::Connection;
use uuid::Uuid;

use crate::{AppConfig, GalleryDb};
use crate::db::schema::{add_image, add_tag, link_image_tag};

#[derive(FromForm)]
pub struct UploadForm<'a> {
    pub file: TempFile<'a>,
    pub tags: String,
}

#[post("/upload", data = "<form>")]
pub async fn file(config: &State<AppConfig>, mut form: Form<UploadForm<'_>>, mut db: Connection<GalleryDb>) -> Result<String, String> {
    let uuid = Uuid::new_v4().to_string();
    let new_filepath = config.gallery_path.join(&uuid);

    if let Err(e) = form.file.copy_to(&new_filepath).await {
        return Err(format!("Failed to save the file: {}", e));
    }

    add_image(&mut **db, &uuid)
        .await
        .map_err(|e| format!("Failed to upload image with uuid '{e}'"))?;

    for tag in form.tags.split(',') {
        let tag = tag.trim().to_lowercase();

        add_tag(&mut **db, &tag)
            .await
            .map_err(|e| format!("Failed to upload tag '{e}'"))?;

        link_image_tag(&mut **db, &uuid, &tag)
            .await
            .map_err(|e| format!("Failed to upload image_tag for image with uuid '{e}'"))?;
    }

    Ok(String::from("Successfully uploaded images and tags"))
}
