#include <raylib.h>
#include "gif.hpp"


int main()
{
    InitWindow(800, 800, "Gifhub");

    Image image       = LoadImage("img.png");
    Texture2D chicken = LoadTextureFromImage(image);
    UnloadImage(image);

    Gif gif = Gif("g.gif", 42);

    float frameTime = GetFrameTime();

    while (!WindowShouldClose()) {
        frameTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(chicken, 0, 0, WHITE);
        DrawTexture(gif.texture, 0, 300, WHITE);

        EndDrawing();

        gif.update(frameTime);
    }

    CloseWindow();
    return 0;
}
