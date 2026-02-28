use rocket::{Build, Rocket};
use rocket_db_pools::{sqlx::SqlitePool, Database};

#[derive(Database)]
#[database("gifhub_db")]
pub struct GifhubDb(SqlitePool);

pub async fn run_migrations(rocket: Rocket<Build>) -> rocket::fairing::Result {
    match GifhubDb::fetch(&rocket) {
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
