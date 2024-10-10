#include <raylib.h>
#include "library.cpp"


int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Gifhub");

    Library library;
    library.addImage("library/img.png");
    library.addGif("library/g.gif");

    float frameTime = 0.0f;
    while (!WindowShouldClose()) {
        frameTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(library.getImages()[0], 0, 0, WHITE);
        DrawTexture(library.getGifs()[0].texture, 0, 200, WHITE);

        EndDrawing();

        library.update(frameTime);
    }

    CloseWindow();
    return 0;
}
