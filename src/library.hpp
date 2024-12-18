#pragma once
#include <vector>
#include <cstdint>

#include "settings.hpp"
#include "utils.hpp"
#include "gif.hpp"


struct Library
{
    typedef struct Item
    {
        // Type info
        Gif gif;
        bool isGif;

        // Image data
        bool isLoaded = false;
        float size[2];
        Texture texture = {};

        Vector2 position = {0.0f, 0.0f};

        std::string path;
    } Item;

    void add(const std::string& filePath, int img_width, int img_height);
    void add(const std::string& filePath, const Image& image);

    void update(const float& frameTime);

    uint64_t librarySize    = 0ULL;
    std::vector<Item> items = {};

    ~Library();
};
