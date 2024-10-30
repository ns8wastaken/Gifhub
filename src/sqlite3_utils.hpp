#pragma once
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>


namespace Sqlite3Utils
{
    bool pathIsinDatabase(sqlite3* db, const char* id)
    {
        sqlite3_stmt* stmt;
        const char* sql = "SELECT * FROM images WHERE id = ?;";

        bool exists = false;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);

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


    std::vector<std::string> getImagePaths(sqlite3* db)
    {
        sqlite3_stmt* stmt;
        const char* sql = "SELECT path FROM images ORDER BY date_added DESC;";

        std::vector<std::string> imagePaths = {};

        // Prepare the SQL statement
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            // Loop over each row
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                const unsigned char* path = sqlite3_column_text(stmt, 0);
                imagePaths.push_back(reinterpret_cast<const char*>(path));
            }
        }
        else {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
        return imagePaths;
    }


    void addImage(sqlite3* db, const char* path, int width, int height) {
        sqlite3_stmt* stmt;
        const char* sql = "INSERT INTO images (path, img_width, img_height) VALUES (?, ?, ?);";

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 2, width);
            sqlite3_bind_int(stmt, 3, height);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "Failed to insert image: " << sqlite3_errmsg(db) << std::endl;
            }
        }
        else {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        }
    }
}
