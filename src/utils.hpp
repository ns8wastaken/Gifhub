#pragma once
#include <raylib.h>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

#include "settings.hpp"


#define CONSOLE_INFO
#define CONSOLE_ERROR


#ifdef CONSOLE_INFO
#define printInfo(msg)  printf("\033[0;32mGifhub\033[0m: %s\n", msg)
#else
#define printInfo(msg)
#endif

#ifdef CONSOLE_ERROR
#define printError(msg) printf("\033[0;31mGifhub\033[0m: %s\n", msg)
#else
#define printError(msg)
#endif


namespace Utils
{
    namespace fs = std::filesystem;


    std::vector<std::string> splitStr(const std::string& str, const char delimiter)
    {
        std::vector<std::string> list;
        std::size_t currIndex = 0;
        std::size_t nextIndex;

        while (currIndex < str.length()) {
            nextIndex = str.find_first_of(delimiter, currIndex);

            if (nextIndex == std::string::npos) {
                list.push_back(str.substr(currIndex));
                break;
            }

            list.push_back(str.substr(currIndex, nextIndex - currIndex));
            currIndex = nextIndex + 1;
        }

        return list;
    }


    // Returns the file paths to all files in a directory
    std::vector<std::string> getFilesInDirectory(const std::string& directoryPath)
    {
        std::vector<std::string> files;

        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file())
                files.push_back(entry.path().string());
        }

        return files;
    }


    inline bool isImage(const std::string& path)
    {
        return path.ends_with(".png") || path.ends_with(".jpg") || path.ends_with(".jpeg");
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

    void ClampImageSize(Vector2& size)
    {
        if (size.x > Settings::MAX_IMAGE_WIDTH || size.y > Settings::MAX_IMAGE_HEIGHT) {
            const float max = std::max(size.x, size.y);

            size.x *= (float)Settings::MAX_IMAGE_WIDTH / max;
            size.y *= (float)Settings::MAX_IMAGE_HEIGHT / max;
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
