#pragma once
#include <raylib.h>
#include <sqlite3.h>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <stack>

#include "settings.hpp"
#include "library.hpp"


class Gifhub
{
public:
    Gifhub(const Color bgColor, const Color frameColor);

    void update(const float& frameTime);
    void draw();

    void assignUniforms(float* screenSize, float* time);

    // Load images in a background thread
    void loadImagesAsync();

    // Process the images in the async queue in the main thread
    void processAsyncQueue();

    ~Gifhub();

private:
    std::stack<std::pair<std::string, Image>> imageQueue = {};
    std::mutex queueMutex;
    std::unordered_map<uint64_t, Texture> m_cachedVisibleTextures = {};

    sqlite3* m_database;

    // Background image loading method (runs in a separate thread)
    void loadImages();

    float m_scroll = 0.0f;

    static constexpr int m_extraImgSpacing = Settings::FRAME_BORDER_WIDTH * 2.0f + Settings::IMAGE_PADDING;

    Color m_bgColor;
    Color m_frameColor;

    // Frame shaders
    Utils::ShaderWrapper m_shader_Frame;
    Utils::ShaderWrapper m_shader_RGBFrame;
    Texture m_shader_TextureBlank;

    Library m_library;

    float* m_screenSize = nullptr;
    float* m_time       = nullptr;
};
