#include <raylib.h>
#include <iostream>

#include "utils.hpp"
#include "library.cpp"
#include "gifhub.cpp"


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "Gifhub");

    Gifhub gifhub(Settings::Theme::LIGHT_NORMAL, Settings::Theme::LIGHT_DARK);

    // Load library
    for (const std::string& filePath : Utils::getFilesInDirectory("library")) {
        gifhub.addToLibrary(TextFormat("%s", filePath.c_str()));
    }

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
