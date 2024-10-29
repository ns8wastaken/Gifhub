#pragma once
#include <sqlite3.h>
#include <iostream>
#include <string>


namespace Sqlite3Utils
{
    bool checkIfIdExists(sqlite3* db, const std::string& id)
    {
        sqlite3_stmt* stmt;
        const char* sql = "SELECT 1 FROM images WHERE id = ? LIMIT 1;";
        bool exists     = false;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                exists = true;  // If a row was found, the ID exists
            }
        }
        else {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
        return exists;
    }
}
