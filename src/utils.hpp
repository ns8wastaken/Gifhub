#pragma once
#include <raylib.h>
#include <filesystem>
#include <string>
#include <vector>

#include "settings.hpp"


namespace Utils
{
    namespace fs = std::filesystem;

    std::vector<std::string> getFilesInDirectory(const std::string& directoryPath)
    {
        std::vector<std::string> files;

        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file())
                files.push_back(entry.path().string());
        }

        return files;
    }


    void ClampImageSize(Image* image)
    {
        if (image->width > Settings::MAX_IMAGE_WIDTH || image->height > Settings::MAX_IMAGE_HEIGHT) {
            const float max = static_cast<float>(std::max(image->width, image->height));
            ImageResize(
                image,
                image->width * static_cast<float>(Settings::MAX_IMAGE_WIDTH / max),
                image->height * static_cast<float>(Settings::MAX_IMAGE_HEIGHT / max)
            );
        }
    }


    void DrawRoundedRectangle(const Vector2 pos, const Vector2 size, const int radius, const Color color)
    {
        DrawRectangle(pos.x, pos.y - radius, size.x, size.y + radius * 2, color); // Top + Bottom
        DrawRectangle(pos.x - radius, pos.y, size.x + radius * 2, size.y, color); // Left + Right
        DrawCircleV(pos, radius, color);                              // Top left
        DrawCircleV({pos.x + size.x, pos.y}, radius, color);          // Top right
        DrawCircleV({pos.x + size.x, pos.y + size.y}, radius, color); // Bottom right
        DrawCircleV({pos.x, pos.y + size.y}, radius, color);          // Bottom left
    }
}
