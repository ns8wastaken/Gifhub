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
    library.add("library/img.png");
    library.add("library/p.png");
    library.add("library/g.gif");

    float screenSize[2] = {
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())
    };

    // Normal frame shader
    Utils::ShaderWrapper shader_Frame;
    {
        shader_Frame.shader = LoadShader(0, "src/shaders/frame.fs");
        shader_Frame.registerUniform("screenResolution");
        shader_Frame.registerUniform("texture");
        shader_Frame.registerUniform("textureSize");
    }

    // RGB frame shader
    Utils::ShaderWrapper shader_RGBFrame;
    {
        shader_RGBFrame.shader = LoadShader(0, "src/shaders/rgb_frame.fs");
        shader_RGBFrame.registerUniform("screenResolution");
        shader_RGBFrame.registerUniform("texture");
        shader_RGBFrame.registerUniform("textureSize");
        shader_RGBFrame.registerUniform("time");
    }

    // Generate blank texture for shader
    Image imBlank               = GenImageColor(screenSize[0], screenSize[1], BLANK);
    Texture shader_TextureBlank = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);

    // Init shader value
    SetShaderValue(shader_Frame.shader, shader_Frame.loc("screenResolution"), &screenSize, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader_RGBFrame.shader, shader_RGBFrame.loc("screenResolution"), &screenSize, SHADER_UNIFORM_VEC2);

    float frameTime   = 0.0f;
    float programTime = 0.0f;
    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            screenSize[0] = static_cast<float>(GetScreenWidth());
            screenSize[1] = static_cast<float>(GetScreenHeight());

            // Update blank texture for shader
            Image imBlank       = GenImageColor(screenSize[0], screenSize[1], BLANK);
            shader_TextureBlank = LoadTextureFromImage(imBlank);
            UnloadImage(imBlank);

            // Update shader values
            SetShaderValue(shader_Frame.shader, shader_Frame.loc("screenResolution"), &screenSize, SHADER_UNIFORM_VEC2);
            SetShaderValue(shader_RGBFrame.shader, shader_RGBFrame.loc("screenResolution"), &screenSize, SHADER_UNIFORM_VEC2);
        }

        frameTime = GetFrameTime();
        programTime += frameTime;

        BeginDrawing();
        {
            ClearBackground(Theme::LIGHT_NORMAL);

            float textureSize[2] = {
                static_cast<float>(library.getItems()[0].size[0]),
                static_cast<float>(library.getItems()[0].size[1])
            };

            // Normal frame shader
            SetShaderValue(shader_Frame.shader, shader_Frame.loc("textureSize"), &textureSize, SHADER_UNIFORM_VEC2);

            // RGB frame shader
            SetShaderValue(shader_RGBFrame.shader, shader_RGBFrame.loc("textureSize"), &textureSize, SHADER_UNIFORM_VEC2);
            SetShaderValue(shader_RGBFrame.shader, shader_RGBFrame.loc("time"), &programTime, SHADER_UNIFORM_FLOAT);

            BeginShaderMode(shader_RGBFrame.shader);
            {
                SetShaderValueTexture(shader_RGBFrame.shader, shader_RGBFrame.loc("texture"), library.getItems()[0].texture);
                DrawTexture(shader_TextureBlank, 0, 0, RED);
            }
            EndShaderMode();
        }
        EndDrawing();

        library.update(frameTime);
    }

    UnloadTexture(shader_TextureBlank);

    CloseWindow();
    return 0;
}
