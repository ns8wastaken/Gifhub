#[derive(serde::Serialize, sqlx::FromRow)]
pub struct DbQueryImage {
    pub uuid: String,
}
