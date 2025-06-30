#[macro_use] extern crate rocket;

mod routes;
mod db;
mod macros;

use db::gallery_db::{GalleryDb, init_db};
use rocket_db_pools::Database;
use routes::{upload, search, reset_files, delete};
use rocket::fs::{relative, FileServer};
use rocket::fairing::AdHoc;

#[launch]
async fn rocket() -> _ {
    std::fs::create_dir_all("gallery")
        .expect("Could not create gallery directory");

    std::fs::OpenOptions::new()
        .write(true)
        .create(true)
        .open("db/gallery.sqlite")
        .expect("Could not create the database at 'db/gallery.sqlite'");

    rocket::build()
        .attach(GalleryDb::init())
        .attach(AdHoc::on_ignite("Init DB", init_db))
        .mount("/", FileServer::from(relative!("public")).rank(1)) // html
        .mount("/", routes![upload::file]) // uploading image
        .mount("/api", routes![
            search::images, // getting a list of all images
            search::search_db, // querying for images
            reset_files::reset // quick reset (deletes db + images)
        ])
        .mount("/gallery", FileServer::from(relative!("gallery"))) // getting individual images
        .mount("/gallery", routes![delete::delete_image]) // delete an image
}
