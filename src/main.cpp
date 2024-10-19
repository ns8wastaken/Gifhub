#include <raylib.h>

#include "utils.hpp"
#include "library.cpp"
#include "gifhub.cpp"


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "Gifhub");

    // sqlite3* db;
    // int rc = sqlite3_open("library/sqlite3/data.sql", &db);

    // if (!rc) {
    //     printf("Opened database successfully\n");
    // }

    Gifhub gifhub(Settings::Theme::LIGHT_NORMAL, Settings::Theme::LIGHT_DARK);
    gifhub.loadImagesAsync();

    float screenSize[2] = {
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())
    };

    float frameTime   = 0.0f;
    float programTime = 0.0f;

    gifhub.assignUniforms(screenSize, &programTime);

    while (!WindowShouldClose()) {
        frameTime = GetFrameTime();
        programTime += frameTime;

        gifhub.processAsyncQueue();

        gifhub.update(frameTime);
        gifhub.draw();
    }

    // asyncImagesThread.join();

    CloseWindow();
    return 0;
}
