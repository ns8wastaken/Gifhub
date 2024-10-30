#pragma once
#include <raylib.h>
#include <sqlite3.h>

#include <thread>
#include <mutex>
#include <stack>

#include "settings.hpp"
#include "library.hpp"
#include "sqlite3_utils.hpp"


extern "C"
{
#include "tinyfiledialogs/tinyfiledialogs.h"
}


class Gifhub
{
public:
    Gifhub(float* time, const Color bgColor, const Color frameColor);

    void update(const float& frameTime);
    void render();

    // Load images in a background thread
    void loadImagesAsync();

    // Process the images in the async queue in the main thread
    void processAsyncQueue();

    ~Gifhub();

private:
    typedef struct QueueItem
    {
        std::string path;
        Image image;
    } QueueItem;

    std::stack<QueueItem> imageQueue = {};
    std::mutex queueMutex;

    std::vector<Library::Item*> m_visibleItems = {};

    sqlite3* m_database;

    // Background image loading methods (run in a separate thread)
    void loadImage(const std::string& filePath); // For manually importing images
    void loadImages(); // Loads images from the library

    float m_scroll = 0.0f;

    static constexpr int m_extraImgSpacing = Settings::FRAME_BORDER_WIDTH * 2.0f + Settings::IMAGE_PADDING;

    Color m_bgColor;
    Color m_frameColor;

    // Frame shaders
    Utils::ShaderWrapper m_shader_Frame;
    Utils::ShaderWrapper m_shader_RGBFrame;
    Texture m_shader_TextureBlank;

    Library m_library;

    float m_screenSize[2] = {
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight())
    };

    float* m_time = nullptr;
};
