#include <raylib.h>

#include "utils.hpp"
#include "library.cpp"
#include "gifhub.cpp"


int main()
{
    // SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 800, "Gifhub");

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

        DrawFPS(0, 0);
    }

    CloseWindow();
    return 0;
}
