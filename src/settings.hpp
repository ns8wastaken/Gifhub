#pragma once


namespace Settings
{
    constexpr const char* const ALLOWED_FILE_TYPES[] = {"*.png", "*.jpg"};


    constexpr int SEARCH_BAR_WIDTH  = 1280;
    constexpr int SEARCH_BAR_HEIGHT = 1280;


    constexpr int MAX_IMAGE_WIDTH  = 300;
    constexpr int MAX_IMAGE_HEIGHT = 300;
    constexpr int IMAGE_PADDING    = 10;

    constexpr float FRAME_BORDER_WIDTH = 5.0f;

    constexpr int GIF_FRAME_DELAY = 42;

    constexpr int IMG_SCREEN_PADDING = 30;


    namespace Theme
    {
        constexpr Color LIGHT_NORMAL = {255, 255, 255, 255};
        constexpr Color LIGHT_DARK   = {156, 156, 156, 255};

        constexpr Color DARK_NORMAL = {90, 90, 90, 255};
        constexpr Color DARK_DARK   = {50, 50, 50, 255};
    }
}
