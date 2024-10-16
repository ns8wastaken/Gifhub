#pragma once
#include <raylib.h>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

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


    struct ShaderWrapper
    {
        Shader shader;
        std::unordered_map<const char*, int> locations;

        void registerUniform(const char* name)
        {
            locations[name] = GetShaderLocation(shader, name);
        }

        // Get the location of a uniform
        const int& loc(const char* uniformName)
        {
            return locations[uniformName];
        }

        ~ShaderWrapper()
        {
            UnloadShader(shader);
        }
    };
}
