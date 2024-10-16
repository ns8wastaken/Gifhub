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

        // General data
        float size[2];
        Texture texture;

        uint64_t id;
    } Item;

    void add(const std::string& fileName);

    void setActive(const int index);

    void update(const float& frameTime);

    const std::vector<Item>& getItems();

    ~Library();

private:
    uint64_t m_librarySize = 0ULL;

    std::vector<Item> m_items = {};
};
