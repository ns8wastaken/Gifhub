#pragma once
#include <raylib.h>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

#include "settings.hpp"


#define printInfo(msg)  printf("\033[0;32mGifhub\033[0m: %s\n", msg)
#define printError(msg) printf("\033[0;31mGifhub\033[0m: %s\n", msg)


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
            const int max = std::max(image->width, image->height);
            ImageResize(
                image,
                image->width * Settings::MAX_IMAGE_WIDTH / max,
                image->height * Settings::MAX_IMAGE_HEIGHT / max
            );
        }
    }


    struct ShaderWrapper
    {
        Shader shader;
        std::unordered_map<const char*, int> locations;

        inline void registerUniform(const char* name)
        {
            locations[name] = GetShaderLocation(shader, name);
        }

        // Returns the location of a uniform
        inline const int& loc(const char* uniformName)
        {
            return locations[uniformName];
        }

        ~ShaderWrapper()
        {
            UnloadShader(shader);
        }
    };
}
