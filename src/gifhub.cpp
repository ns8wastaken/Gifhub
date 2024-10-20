#include "gifhub.hpp"


Gifhub::Gifhub(const Color bgColor, const Color frameColor)
    : m_bgColor(bgColor), m_frameColor(frameColor)
{
    // Open database
    int rc = sqlite3_open_v2("library/sqlite3/data.db", &m_database, SQLITE_OPEN_READWRITE, nullptr);

    if (rc == SQLITE_OK) {
        printInfo("Opened library database successfully");
    }
    else {
        printError("Failed to open database");
        this->~Gifhub();
        throw std::runtime_error("Failed to open database");
    }

    // Normal frame shader
    m_shader_Frame.shader = LoadShader(0, "src/shaders/frame.fs");
    m_shader_Frame.registerUniform("screenResolution");
    m_shader_Frame.registerUniform("texture");
    m_shader_Frame.registerUniform("textureSize");
    SetShaderValue(m_shader_Frame.shader, GetShaderLocation(m_shader_Frame.shader, "borderThickness"), &Settings::FRAME_BORDER_WIDTH, SHADER_UNIFORM_FLOAT);

    // RGB frame shader
    m_shader_RGBFrame.shader = LoadShader(0, "src/shaders/rgb_frame.fs");
    m_shader_RGBFrame.registerUniform("screenResolution");
    m_shader_RGBFrame.registerUniform("texture");
    m_shader_RGBFrame.registerUniform("textureSize");
    m_shader_RGBFrame.registerUniform("time");
    SetShaderValue(m_shader_RGBFrame.shader, GetShaderLocation(m_shader_RGBFrame.shader, "borderThickness"), &Settings::FRAME_BORDER_WIDTH, SHADER_UNIFORM_FLOAT);
}


void Gifhub::loadImages()
{
    for (const std::string& filePath : Utils::getFilesInDirectory("library")) {
        Image image = LoadImage(filePath.c_str());
        Utils::ClampImageSize(&image);

        std::lock_guard<std::mutex> lock(queueMutex);
        imageQueue.push({filePath, image});
    }
}


void Gifhub::loadImagesAsync()
{
    std::thread loaderThread(&loadImages, this);
    loaderThread.detach();  // Detach the thread so it runs independently
}


void Gifhub::processAsyncQueue()
{
    std::lock_guard<std::mutex> lock(queueMutex);
    while (!imageQueue.empty()) {
        auto& [fileName, image] = imageQueue.top();

        m_library.add(fileName, image);

        UnloadImage(image);
        imageQueue.pop();
    }
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
    Vector2 currentImagePos = {Settings::IMG_SCREEN_PADDING, m_scroll + Settings::IMG_SCREEN_PADDING};

    BeginDrawing();
    {
        ClearBackground(m_bgColor);

        for (const Library::Item& item : m_library.getItems()) {
            float textureSize[2] = {
                static_cast<float>(item.size[0]),
                static_cast<float>(item.size[1])
            };

            if ((currentImagePos.y + item.size[1] < 0) || (currentImagePos.y > m_screenSize[1])) {
                goto nodraw;
            }

            if (m_cachedVisibleTextures.count(item.id) == 0) {
                m_cachedVisibleTextures[item.id] = LoadTexture(item.path.c_str());
            }

            // Is mouse hovering over image
            if (CheckCollisionPointRec(
                    GetMousePosition(),
                    Rectangle{
                        currentImagePos.x,
                        currentImagePos.y,
                        item.size[0] + Settings::FRAME_BORDER_WIDTH * 2.0f,
                        item.size[1] + Settings::FRAME_BORDER_WIDTH * 2.0f
                    }
                )) {
                SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("textureSize"), &textureSize, SHADER_UNIFORM_VEC2);
                SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("time"), m_time, SHADER_UNIFORM_FLOAT);

                BeginShaderMode(m_shader_RGBFrame.shader);
                {
                    SetShaderValueTexture(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("texture"), m_cachedVisibleTextures[item.id]);
                    DrawTextureEx(m_shader_TextureBlank, currentImagePos, 0, 1, m_frameColor);
                }
                EndShaderMode();
            }
            else {
                SetShaderValue(m_shader_Frame.shader, m_shader_Frame.loc("textureSize"), &textureSize, SHADER_UNIFORM_VEC2);

                BeginShaderMode(m_shader_Frame.shader);
                {
                    SetShaderValueTexture(m_shader_Frame.shader, m_shader_Frame.loc("texture"), m_cachedVisibleTextures[item.id]);
                    DrawTextureEx(m_shader_TextureBlank, currentImagePos, 0, 1, m_frameColor);
                }
                EndShaderMode();
            }

        nodraw:
            currentImagePos.x += item.size[0] + m_extraImgSpacing;

            if (currentImagePos.x + item.size[0] >= m_screenSize[0] - Settings::IMG_SCREEN_PADDING) {
                currentImagePos.x = Settings::IMG_SCREEN_PADDING;
                currentImagePos.y += Settings::FRAME_BORDER_WIDTH * 2.0f + Settings::MAX_IMAGE_HEIGHT + Settings::IMAGE_PADDING;
            }
        }
    }
    EndDrawing();
}


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
    sqlite3_close(m_database);
}
