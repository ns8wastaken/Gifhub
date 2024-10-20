#pragma once
#include <vector>
#include <cstdint>
#include <cstring>

#include "settings.hpp"
#include "utils.hpp"
#include "gif.hpp"


typedef union Media
{
    Gif gif;
    Image img;
    ~Media() {};
} Media;


class Library
{
public:
    typedef struct Item
    {
        // Type info
        Media media;
        bool isGif;

        // Image data
        float size[2];
        Texture texture;

        uint64_t id;
        std::string path;
    } Item;

    void add(const std::string& fileName);
    void add(const std::string& fileName, Image& image);

    void update(const float& frameTime);

    const std::vector<Item>& getItems() const;

    ~Library();

private:
    uint64_t m_librarySize = 0ULL;

    std::vector<Item> m_items = {};
};
