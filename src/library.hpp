#pragma once
#include <vector>
#include <cstdint>

#include "gif.hpp"


class Library
{
public:
    void addImage(const char* fileName);
    void addGif(const char* fileName, const float gifDelay);

    void update(const float& frameTime);

    const std::vector<Image>& getImages();
    const std::vector<Gif>& getGifs();

private:
    uint64_t librarySize = 0ULL;
    std::vector<Image> imgs = {};
    std::vector<Gif> gifs   = {};
};
