#include <raylib.h>
#include <iostream>
#include <filesystem>

#include "utils.hpp"
#include "library.cpp"


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

    Library library;
    library.addImage("library/p.png");
    library.addGif("library/g.gif");

    Vector2 screenSize = {
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())
    };

    // Load shader
    Shader shader                 = LoadShader(0, "src/shaders/rounded_frame.fs");
    const int screenResolutionLoc = GetShaderLocation(shader, "screenResolution");

    // Generate blank texture for shader
    Image imBlank        = GenImageColor(Settings::MAX_IMAGE_WIDTH, Settings::MAX_IMAGE_HEIGHT, BLANK);
    Texture textureBlank = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);

    float frameTime = 0.0f;
    while (!WindowShouldClose()) {
        SetShaderValue(shader, screenResolutionLoc, &screenSize, SHADER_UNIFORM_VEC2);

        frameTime = GetFrameTime();

        BeginDrawing();
        {
            ClearBackground(Theme::LIGHT_NORMAL);

            DrawTexture(library.getImages()[0], 0, 0, WHITE);
            DrawTexture(library.getGifs()[0].texture, 0, 200, WHITE);

            // Utils::DrawRoundedRectangle({300, 100}, {200, 100}, 6, Theme::LIGHT_DARK);
            // DrawRectangle(300, 100, 200, 100, RED);

            BeginShaderMode(shader);
            {
                DrawTexture(textureBlank, 300, 100, RED);
            }
            EndShaderMode();
        }
        EndDrawing();

        library.update(frameTime);
    }

    CloseWindow();
    return 0;
}
