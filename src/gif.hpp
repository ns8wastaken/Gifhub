#pragma once
#include <raylib.h>


struct Gif
{
    Gif(const char* fileName, const int frameDelay) : img(LoadImageAnim(fileName, &frameCount)), delay(frameDelay) {}

    float delayCount = 0;
    int frameCount   = 0;
    int currentFrame = 0;

    const Image img;
    Texture texture = LoadTextureFromImage(img);
    const float delay;

    const unsigned int memOffset = img.width * img.height * 4;

    void update(const float& frameTime)
    {
        delayCount += frameTime * 1000;

        if (delayCount > delay) {
            delayCount   = 0;
            currentFrame = (currentFrame + 1) % frameCount;
            UpdateTexture(texture, (unsigned char*)img.data + currentFrame * memOffset);
        }
    }
};
