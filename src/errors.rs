use rocket::http::Status;
use rocket::response::{self, Responder};
use rocket::Request;

use crate::db::error::DbError;

#[derive(Debug)]
pub enum ApiError {
    Database(DbError),
    FileWrite(std::io::Error),
}

impl<'r> Responder<'r, 'static> for ApiError {
    fn respond_to(self, req: &'r Request<'_>) -> response::Result<'static> {
        let (status, message) = match self {
            ApiError::Database(e)  => (Status::InternalServerError, format!("Database error: {e}")),
            ApiError::FileWrite(e) => (Status::InternalServerError, format!("File error: {e}")),
        };

        rocket::response::status::Custom(status, message).respond_to(req)
    }
}

impl From<DbError> for ApiError {
    fn from(e: DbError) -> Self {
        ApiError::Database(e)
    }
}

impl From<std::io::Error> for ApiError {
    fn from(e: std::io::Error) -> Self {
        ApiError::FileWrite(e)
    }
}

impl From<sqlx::Error> for ApiError {
    fn from(e: sqlx::Error) -> Self {
        ApiError::Database(DbError::Sqlx(e))
    }
}

impl From<sqlx::migrate::MigrateError> for ApiError {
    fn from(e: sqlx::migrate::MigrateError) -> Self {
        ApiError::Database(DbError::Migration(e))
    }
}
