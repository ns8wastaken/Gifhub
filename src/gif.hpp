#pragma once
#include <raylib.h>

#include "utils.hpp"


struct Gif
{
    Gif(const char* fileName) : img(LoadImageAnim(fileName, &frameCount)) {}

    float delayCount = 0; // Current delay count
    int currentFrame = 0;

    const Image img;
    int frameCount; // Number of frames in the gif

    const unsigned int memOffset = img.width * img.height * 4;

    void update(const float& frameTime, Texture& texture)
    {
        delayCount += frameTime * 1000.0f;

        if (delayCount > Settings::GIF_FRAME_DELAY) {
            delayCount   = 0;
            currentFrame = (currentFrame + 1) % frameCount;
            UpdateTexture(texture, (unsigned char*)img.data + currentFrame * memOffset);
        }
    }
};
