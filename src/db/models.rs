#[derive(serde::Serialize, sqlx::FromRow)]
pub struct DbQueryImageUUID {
    pub uuid: String,
}
