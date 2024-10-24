#include <raylib.h>

#include "utils.hpp"
#include "library.cpp"
#include "gifhub.cpp"


extern "C"
{
#include "tinyfiledialogs/tinyfiledialogs.h"
}


int main()
{
    const char* const allowedFileTypes[2] = {"*.png", "*.jpg"};

    // const char* filePaths = tinyfd_openFileDialog("Import Image or Gif", "", 2, allowedFileTypes, "Allowed files", 1);
    // for (const std::string& path : Utils::splitStr(filePaths, '|')) {
    // }

    // SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "Gifhub");

    Gifhub gifhub(Settings::Theme::LIGHT_NORMAL, Settings::Theme::LIGHT_DARK);
    gifhub.loadImagesAsync();

    float frameTime   = 0.0f;
    float programTime = 0.0f;

    gifhub.assignUniforms(&programTime);

    while (!WindowShouldClose()) {
        frameTime = GetFrameTime();
        programTime += frameTime;

        gifhub.processAsyncQueue();

        gifhub.update(frameTime);
        gifhub.draw();

        DrawFPS(0, 0);
    }

    CloseWindow();
    return 0;
}
