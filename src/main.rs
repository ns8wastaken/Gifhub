#[macro_use] extern crate rocket;

mod routes;
mod db;

use std::path::PathBuf;
use rocket_db_pools::Database;
use routes::{upload, search, delete, nuke_files, tags};
use rocket::fs::{relative, FileServer};
use rocket::fairing::AdHoc;
use rocket::figment::value::{Map, Value};
use db::gallery_db::{GalleryDb, run_migrations};

pub struct AppConfig {
    pub gallery_path: PathBuf,
    pub db_path: PathBuf,
}

#[launch]
async fn rocket() -> _ {
    let config = AppConfig {
        gallery_path: PathBuf::from("./data/storage"),
        db_path: PathBuf::from("./data/db"),
    };

    assert!(config.gallery_path.extension().is_none());
    assert!(config.db_path.extension().is_none());

    // Create gallery and db dirs
    std::fs::create_dir_all(&config.gallery_path)
        .expect("Could not create gallery directory");
    std::fs::create_dir_all(&config.db_path)
        .expect("Could not create db directory");

    // Create db file
    let full_db_path = config.db_path.join("gallery.sqlite");
    std::fs::OpenOptions::new()
        .write(true)
        .create(true)
        .open(&full_db_path)
        .expect(&format!("Could not create the database at '{}'", full_db_path.to_string_lossy()));

    let mut db_config: Map<String, Value> = Map::new();
    db_config.insert(
        String::from("url"),
        Value::from(full_db_path.to_string_lossy().to_string())
    );

    // Add db path to Rocket
    let figment = rocket::Config::figment()
        .merge(("databases.gallery_db", db_config));

    let migration_fairing = AdHoc::try_on_ignite("SQLx Migrations", run_migrations);

    rocket::custom(figment)
        .attach(GalleryDb::init())
        .attach(migration_fairing)
        .mount("/", FileServer::from(relative!("public")).rank(1)) // html
        .mount("/", routes![upload::file]) // uploading image
        .mount("/api", routes![
            search::images,    // getting a list of all images
            search::search_db, // querying for images
            nuke_files::nuke   // deletes db + images
        ])
        .mount("/gallery", FileServer::from(&config.gallery_path)) // getting individual images
        .mount("/gallery", routes![delete::delete_image]) // delete an image
        .mount("/gallery", routes![tags::edit_tags, tags::get_tags]) // edit/get the tags of an image
        .manage(config)
}
