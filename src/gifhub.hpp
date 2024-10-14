#pragma once
#include <raylib.h>

#include "settings.hpp"
#include "library.hpp"


class Gifhub
{
public:
    Gifhub(const Color bgColor, const Color frameColor);

    void update(const float& frameTime);
    void draw();
    void addToLibrary(const std::string& fileName);
    const std::vector<Library::Item>& getItems();

    void assignUniforms(float* screenSize, float* time);

    ~Gifhub();

private:
    Color m_bgColor;
    Color m_frameColor;

    Utils::ShaderWrapper m_shader_Frame;
    Utils::ShaderWrapper m_shader_RGBFrame;
    Texture m_shader_TextureBlank;

    Library m_library;

    float* m_screenSize = nullptr;
    float* m_time       = nullptr;
};