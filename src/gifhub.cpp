#include "gifhub.hpp"


Gifhub::Gifhub(float* time, const Color bgColor, const Color frameColor)
    : m_bgColor(bgColor), m_frameColor(frameColor), m_time(time)
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

    // Init database
    sqlite3_exec(
        m_database,
        "CREATE TABLE IF NOT EXISTS images ("
        "path TEXT NOT NULL PRIMARY KEY,"
        "img_width INTEGER NOT NULL,"
        "img_height INTEGER NOT NULL,"
        "date_added DATETIME DEFAULT CURRENT_TIMESTAMP);",
        nullptr,
        nullptr,
        nullptr
    );

    // sqlite3_exec(
    //     m_database,
    //     "INSERT INTO images (path, img_width, img_height) VALUES ('library/soy-sauce.png', 100, 40);",
    //     nullptr,
    //     nullptr,
    //     nullptr
    // );

    // Generate blank texture for shader
    Image imBlank         = GenImageColor(m_screenSize[0], m_screenSize[1], m_bgColor);
    m_shader_TextureBlank = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);

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


    SetShaderValue(m_shader_Frame.shader, m_shader_Frame.loc("screenResolution"), m_screenSize, SHADER_UNIFORM_VEC2);
    SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("screenResolution"), m_screenSize, SHADER_UNIFORM_VEC2);
}


void Gifhub::loadImage(const std::string& filePath)
{
    Image image = LoadImage(filePath.c_str());
    Utils::ClampImageSize(&image);

    ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    std::lock_guard<std::mutex> lock(queueMutex);
    imageQueue.push({filePath, image});
}


void Gifhub::loadImages()
{
    for (const std::string& filePath : Utils::getFilesInDirectory("library")) {
        if (Sqlite3Utils::pathIsinDatabase(m_database, filePath.c_str())) {
            Vector2 size = Sqlite3Utils::getImageSize(m_database, filePath.c_str());

            imageQueue.push(QueueItem{
                .path        = filePath,
                .data        = {.size = size},
                .isInLibrary = true
            });

            continue;
        }

        Image image = LoadImage(filePath.c_str());
        Utils::ClampImageSize(&image);

        ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

        std::lock_guard<std::mutex> lock(queueMutex);
        imageQueue.push(QueueItem{
            .path = filePath,
            .data = {.image = image}
        });
    }
}


void Gifhub::loadImagesAsync()
{
    std::thread loaderThread(&loadImages, this);
    loaderThread.detach();
}


void Gifhub::processAsyncQueue()
{
    std::lock_guard<std::mutex> lock(queueMutex);
    while (!imageQueue.empty()) {
        QueueItem& item = imageQueue.top();

        if (!item.isInLibrary) {
            // TODO: Add item to database
        }

        m_library.add(item.path, item.image);

        UnloadImage(item.image);
        imageQueue.pop();
    }
}


void Gifhub::update(const float& frameTime)
{
    // Update library (gif frames)
    m_library.update(frameTime);


    // Update scroll
    m_scroll += GetMouseWheelMove() * 30.0f;


    // Update screen size and related values
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


    // Update visible items (locations + visibility)
    m_visibleItems.clear();

    Vector2 currentImagePos = {Settings::IMG_SCREEN_PADDING, m_scroll + Settings::IMG_SCREEN_PADDING};
    for (Library::Item& item : m_library.items) {
        if ((currentImagePos.y + item.size[1] >= 0) && (currentImagePos.y < m_screenSize[1])) {
            // Image is visible
            // Load image if unloaded
            if (!item.isLoaded) {
                item.texture  = LoadTexture(item.path.c_str());
                item.isLoaded = true;
            }

            item.position = currentImagePos;
            m_visibleItems.push_back((Library::Item*)(&item));
        }
        else {
            // Image is not visible
            // Unload image if loaded
            if (item.isLoaded) {
                UnloadTexture(item.texture);
                item.isLoaded = false;
            }
        }

        currentImagePos.x += item.size[0] + m_extraImgSpacing;

        if (currentImagePos.x + item.size[0] >= m_screenSize[0] - Settings::IMG_SCREEN_PADDING) {
            currentImagePos.x = Settings::IMG_SCREEN_PADDING;
            currentImagePos.y += Settings::FRAME_BORDER_WIDTH * 2.0f + Settings::MAX_IMAGE_HEIGHT + Settings::IMAGE_PADDING;
        }
    }


    // Import images (user input)
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_O)) {
        const char* filePaths = tinyfd_openFileDialog("Import Image or Gif", "", 2, Settings::ALLOWED_FILE_TYPES, "Allowed files", 1);

        printf("filePaths: %s\n", filePaths);

        for (const std::string& path : Utils::splitStr(filePaths, '|')) {
            loadImage(path);
        }
    }
}


void Gifhub::render()
{
    BeginDrawing();
    ClearBackground(m_bgColor);

    for (const Library::Item* item : m_visibleItems) {
        // Is mouse hovering over image
        if (CheckCollisionPointRec(
                GetMousePosition(),
                Rectangle{
                    item->position.x,
                    item->position.y,
                    item->size[0] + Settings::FRAME_BORDER_WIDTH * 2.0f,
                    item->size[1] + Settings::FRAME_BORDER_WIDTH * 2.0f
                }
            )) {
            SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("textureSize"), &(item->size), SHADER_UNIFORM_VEC2);
            SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("time"), m_time, SHADER_UNIFORM_FLOAT);

            // Draw RGB frame
            BeginShaderMode(m_shader_RGBFrame.shader);
            SetShaderValueTexture(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("texture"), item->texture);
            DrawTextureEx(m_shader_TextureBlank, item->position, 0, 1, m_frameColor);
            EndShaderMode();
        }
        else {
            SetShaderValue(m_shader_Frame.shader, m_shader_Frame.loc("textureSize"), &(item->size), SHADER_UNIFORM_VEC2);

            // Draw normal frame
            BeginShaderMode(m_shader_Frame.shader);
            SetShaderValueTexture(m_shader_Frame.shader, m_shader_Frame.loc("texture"), item->texture);
            DrawTextureEx(m_shader_TextureBlank, item->position, 0, 1, m_frameColor);
            EndShaderMode();
        }
    }

    if (IsKeyDown(KEY_TAB)) {
        float size[2] = {
            m_screenSize[0] * 0.85f,
            m_screenSize[1] * 0.85f
        };

        SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("textureSize"), &size, SHADER_UNIFORM_VEC2);
        SetShaderValue(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("time"), m_time, SHADER_UNIFORM_FLOAT);

        BeginShaderMode(m_shader_RGBFrame.shader);
        SetShaderValueTexture(m_shader_RGBFrame.shader, m_shader_RGBFrame.loc("texture"), m_shader_TextureBlank);
        DrawTextureEx(
            m_shader_TextureBlank,
            Vector2{
                (m_screenSize[0] - size[0]) * 0.5f,
                (m_screenSize[1] - size[1]) * 0.5f
            },
            0,
            1,
            m_frameColor
        );
        EndShaderMode();
    }

    EndDrawing();
}


Gifhub::~Gifhub()
{
    UnloadTexture(m_shader_TextureBlank);
    // UnloadTexture(m_infoTexture);

    sqlite3_close(m_database);
}
