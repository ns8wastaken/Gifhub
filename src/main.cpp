#include <raylib.h>

#include "utils.hpp"
#include "library.cpp"
#include "gifhub.cpp"


int main()
{
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "Gifhub");

    float frameTime   = 0.0f;
    float programTime = 0.0f;

    Gifhub gifhub(&programTime, Settings::Theme::LIGHT_NORMAL, Settings::Theme::LIGHT_DARK);
    gifhub.loadImagesAsync();

    while (!WindowShouldClose()) {
        frameTime = GetFrameTime();
        programTime += frameTime;

        gifhub.processAsyncQueue();

        gifhub.update(frameTime);
        gifhub.render();

        DrawFPS(0, 0);
    }

    CloseWindow();
    return 0;
}
