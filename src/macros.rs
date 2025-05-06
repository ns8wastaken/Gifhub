#[macro_export]
macro_rules! sql_exec {
    ($db:expr, $sql:expr $(, $bind:expr )* ) => {
        sqlx::query($sql)
            $(.bind($bind))*
            .execute(&mut **$db)
            .await
    };
}

#[macro_export]
macro_rules! sql_exec_map_err {
    ($db:expr, $sql:expr, $err:expr $(, $bind:expr)* ) => {
        sqlx::query($sql)
            $(.bind($bind))*
            .execute(&mut **$db)
            .await
            .map_err(|e| format!("{}: {}", $err, e))
    };
}
