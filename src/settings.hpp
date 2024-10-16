#pragma once


namespace Settings
{
    constexpr int MAX_IMAGE_WIDTH  = 300;
    constexpr int MAX_IMAGE_HEIGHT = 300;
    constexpr int IMAGE_PADDING    = 10;


    constexpr int GIF_FRAME_DELAY = 42;

    constexpr int FRAME_BORDER_WIDTH    = 5;
    constexpr float FRAME_BORDER_RADIUS = 20.0f;


    namespace Theme
    {
        constexpr Color LIGHT_NORMAL = {255, 255, 255, 255};
        constexpr Color LIGHT_DARK   = {156, 156, 156, 255};

        constexpr Color DARK_NORMAL = {90, 90, 90, 255};
        constexpr Color DARK_DARK   = {50, 50, 50, 255};
    }
}
