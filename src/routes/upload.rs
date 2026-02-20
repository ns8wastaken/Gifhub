use rocket::form::Form;
use rocket::fs::TempFile;
use rocket_db_pools::Connection;
use uuid::Uuid;

use crate::{sql_exec_map_err, GalleryDb};
use crate::db::schema::{ADD_IMAGE, ADD_TAG, ADD_IMAGE_TAG};

#[derive(FromForm)]
pub struct UploadForm<'a> {
    pub file: TempFile<'a>,
    pub tags: String,
}

#[post("/upload", data = "<form>")]
pub async fn file(mut form: Form<UploadForm<'_>>, mut db: Connection<GalleryDb>) -> Result<String, String> {
    let uuid = Uuid::new_v4().to_string();
    let new_filepath = format!("gallery/{}", uuid);

    if let Err(e) = form.file.copy_to(&new_filepath).await {
        return Err(format!("Failed to save the file: {}", e));
    }

    sql_exec_map_err!(
        db,
        ADD_IMAGE,
        format!("Failed to upload image with uuid '{}'", &uuid),
        &uuid
    )?;

    for tag in form.tags.split(',') {
        let tag = tag.trim().to_lowercase();

        sql_exec_map_err!(
            db,
            ADD_TAG,
            format!("Failed to upload tag '{}'", &tag),
            &tag
        )?;
        sql_exec_map_err!(
            db,
            ADD_IMAGE_TAG,
            format!("Failed to upload image_tag for image with uuid '{}'", &uuid),
            &uuid,
            &tag
        )?;
    }

    Ok(String::from("Successfully uploaded images and tags"))
}
