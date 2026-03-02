use rocket::serde::Serialize;

#[derive(Serialize, sqlx::FromRow)]
pub struct DbQueryImageUUID {
    pub uuid: String,
}

#[derive(Serialize, sqlx::FromRow)]
pub struct DbQueryImage {
    pub uuid: String,
    pub tags: Vec<String>,
}

#[derive(Serialize, sqlx::FromRow)]
pub struct TagItem {
    pub value: String,
    pub count: i64,
}
