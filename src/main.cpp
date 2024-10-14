#include <raylib.h>
#include <iostream>
#include <filesystem>

#include "utils.hpp"
#include "library.cpp"
#include "gifhub.cpp"


namespace Theme
{
    constexpr Color LIGHT_NORMAL = {255, 255, 255, 255};
    constexpr Color LIGHT_DARK   = {156, 156, 156, 255};

    constexpr Color DARK_NORMAL = {90, 90, 90, 255};
    constexpr Color DARK_DARK   = {50, 50, 50, 255};
}


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "Gifhub");

    Gifhub gifhub(Theme::LIGHT_NORMAL, Theme::LIGHT_DARK);
    gifhub.addToLibrary("library/p.png");
    gifhub.addToLibrary("library/img.png");
    gifhub.addToLibrary("library/g.gif");

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

        gifhub.update(frameTime);
        gifhub.draw();
    }

    CloseWindow();
    return 0;
}
