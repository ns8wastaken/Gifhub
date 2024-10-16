#include "gifhub.hpp"


Gifhub::Gifhub(const Color bgColor, const Color frameColor)
    : m_bgColor(bgColor), m_frameColor(frameColor)
{
    // Normal frame shader
    m_shader_Frame.shader = LoadShader(0, "src/shaders/frame.fs");
    m_shader_Frame.registerUniform("screenResolution");
    m_shader_Frame.registerUniform("texture");
    m_shader_Frame.registerUniform("textureSize");
    // SetShaderValue(m_shader_Frame.shader, GetShaderLocation(m_shader_Frame.shader, "radius"), &Settings::FRAME_BORDER_RADIUS, SHADER_UNIFORM_FLOAT);
    // SetShaderValue(m_shader_Frame.shader, GetShaderLocation(m_shader_Frame.shader, "borderThickness"), &Settings::FRAME_BORDER_WIDTH, SHADER_UNIFORM_FLOAT);

    // RGB frame shader
    m_shader_RGBFrame.shader = LoadShader(0, "src/shaders/rgb_frame.fs");
    m_shader_RGBFrame.registerUniform("screenResolution");
    m_shader_RGBFrame.registerUniform("texture");
    m_shader_RGBFrame.registerUniform("textureSize");
    m_shader_RGBFrame.registerUniform("time");
}


void Gifhub::update(const float& frameTime)
{
    m_library.update(frameTime);

    m_scroll += GetMouseWheelMove() * 30.0f;

    if (IsWindowResized()) {
        m_screenSize[0] = static_cast<float>(GetScreenWidth());
        m_screenSize[1] = static_cast<float>(GetScreenHeight());

        // Update blank texture for shader
        UnloadTexture(m_shader_TextureBlank);
        Image imBlank         = GenImageColor(m_screenSize[0], m_screenSize[1], BLANK);
        m_shader_TextureBlank = LoadTextureFromImage(imBlank);
        UnloadImage(imBlank);

        // Update shader values
        SetShaderValue(m_shader_Frame.shader, m_shader_Frame.loc("screenResolution"), m_screenSize, SHADER_UNIFORM_VEC2);
        SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("screenResolution"), m_screenSize, SHADER_UNIFORM_VEC2);
    }
}


void Gifhub::draw()
{
    Vector2 currentImagePos = {0.0f, m_scroll};

    BeginDrawing();
    {
        ClearBackground(m_bgColor);

        for (const Library::Item& item : getItems()) {

            float textureSize[2] = {
                static_cast<float>(item.size[0]),
                static_cast<float>(item.size[1])
            };

            // Normal frame shader
            SetShaderValue(m_shader_Frame.shader, m_shader_Frame.loc("textureSize"), &textureSize, SHADER_UNIFORM_VEC2);
            // RGB frame shader
            SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("textureSize"), &textureSize, SHADER_UNIFORM_VEC2);
            SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("time"), m_time, SHADER_UNIFORM_FLOAT);

            if (CheckCollisionPointRec(GetMousePosition(), {currentImagePos.x, currentImagePos.y, item.size[0], item.size[1]})) {
                m_shader_active = &m_shader_RGBFrame;
            }
            BeginShaderMode(m_shader_active->shader);
            {
                SetShaderValueTexture(m_shader_active->shader, m_shader_active->loc("texture"), item.texture);
                DrawTextureEx(m_shader_TextureBlank, currentImagePos, 0, 1, m_frameColor);
            }
            EndShaderMode();
            m_shader_active = &m_shader_Frame;

            currentImagePos.x += item.size[0] + m_extraImgSpacing;

            if (currentImagePos.x + item.size[0] >= m_screenSize[0]) {
                currentImagePos.x = 0;
                currentImagePos.y += Settings::MAX_IMAGE_HEIGHT + Settings::FRAME_BORDER_WIDTH * 2 + Settings::IMAGE_PADDING;
            }
        }
    }
    EndDrawing();
}


void Gifhub::addToLibrary(const std::string& fileName) { m_library.add(fileName); }


const std::vector<Library::Item>& Gifhub::getItems() { return m_library.getItems(); }


void Gifhub::assignUniforms(float* screenSize, float* time)
{
    m_screenSize = screenSize;
    m_time       = time;

    // Generate blank texture for shader
    Image imBlank         = GenImageColor(m_screenSize[0], m_screenSize[1], BLANK);
    m_shader_TextureBlank = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);

    SetShaderValue(m_shader_Frame.shader, m_shader_Frame.loc("screenResolution"), m_screenSize, SHADER_UNIFORM_VEC2);
    SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("screenResolution"), m_screenSize, SHADER_UNIFORM_VEC2);
}


Gifhub::~Gifhub()
{
    UnloadTexture(m_shader_TextureBlank);
}
