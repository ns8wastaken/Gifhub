use rocket::{Build, Rocket};
use rocket_db_pools::{sqlx::{Executor, SqlitePool}, Database};

use crate::db::schema::{
    INIT_DB_IMAGES,
    INIT_DB_TAGS,
    INIT_DB_IMAGE_TAGS
};

#[derive(Database)]
#[database("gallery_db")]
pub struct GalleryDb(SqlitePool);

impl GalleryDb {
    pub async fn init_gallery_db(&self) -> Result<(), sqlx::Error> {
        self.execute(INIT_DB_IMAGES).await?;
        self.execute(INIT_DB_TAGS).await?;
        self.execute(INIT_DB_IMAGE_TAGS).await?;
        Ok(())
    }
}

pub async fn init_db(rocket: Rocket<Build>) -> Rocket<Build> {
    let db = GalleryDb::fetch(&rocket)
        .expect("Failed to fetch GalleryDb");

    db.init_gallery_db()
        .await
        .expect("GalleryDb failed to initialize");

    rocket
}
