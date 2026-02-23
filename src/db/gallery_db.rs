use rocket::{Build, Rocket};
use rocket_db_pools::{sqlx::SqlitePool, Database};

#[derive(Database)]
#[database("gallery_db")]
pub struct GalleryDb(SqlitePool);

pub async fn run_migrations(rocket: Rocket<Build>) -> rocket::fairing::Result {
    match GalleryDb::fetch(&rocket) {
        Some(db) => match sqlx::migrate!("./migrations").run(&**db).await {
            Ok(_) => Ok(rocket),
            Err(e) => {
                error!("Failed to run database migrations: {e}");
                Err(rocket)
            }
        },
        None => Err(rocket),
    }
}
