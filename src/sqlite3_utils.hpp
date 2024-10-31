#pragma once
#include <sqlite3.h>
#include <raylib.h>
#include <iostream>
#include <string>
#include <vector>


namespace Sqlite3Utils
{
    bool pathIsinDatabase(sqlite3* db, const char* path)
    {
        sqlite3_stmt* stmt;
        const char* sql = "SELECT * FROM images WHERE path = ?;";

        bool exists = false;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                exists = true;  // If a row was found, the path exists
            }
        }
        else {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
        return exists;
    }


    Vector2 getImageSize(sqlite3* db, const char* path)
    {
        Vector2 size = {0.0f, 0.0f};

        sqlite3_stmt* stmt;
        const char* sql = "SELECT img_width, img_height FROM images WHERE path = ?;";

        std::vector<std::string> imagePaths = {};

        // Prepare the SQL statement
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);

            // Loop over each row
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                size.x = sqlite3_column_int(stmt, 0);
                size.y = sqlite3_column_int(stmt, 1);
            }
        }
        else {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
        return size;
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
                const char* path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                imagePaths.push_back(path);
            }
        }
        else {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
        return imagePaths;
    }


    void addImageToDatabase(sqlite3* db, const char* path, int img_width, int img_height)
    {
        sqlite3_stmt* stmt;
        const char* sql = "INSERT INTO images (path, img_width, img_height) VALUES (?, ?, ?);";

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 2, img_width);
            sqlite3_bind_int(stmt, 3, img_height);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "Failed to insert image: " << sqlite3_errmsg(db) << std::endl;
            }
        }
        else {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        }
    }
}
