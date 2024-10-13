#include "library.hpp"


void Library::add(const std::string& fileName)
{
    ++librarySize;

    Image image = LoadImage(fileName.c_str());
    Utils::ClampImageSize(&image);

    if (fileName.ends_with(".gif")) {
        items.push_back(Item{
            .media   = {.gif = Gif(fileName.c_str())},
            .isGif   = true,
            .size    = {static_cast<float>(image.width), static_cast<float>(image.height)},
            .texture = LoadTextureFromImage(image)
        });
    }
    else {
        items.push_back(Item{
            .media   = {.img = image},
            .isGif   = false,
            .size    = {static_cast<float>(image.width), static_cast<float>(image.height)},
            .texture = LoadTextureFromImage(image)
        });
    }

    UnloadImage(image);
}


void Library::update(const float& frameTime)
{
    for (Item& item : items) {
        // Update gif frame
        if (item.isGif) {
            item.media.gif.update(frameTime, item.texture);
        }
    }
}


const std::vector<Library::Item>& Library::getItems()
{
    return items;
}


Library::~Library()
{
    // for (Item& item : items) {
    //     if (!item.isGif) UnloadTexture(item.media.img);
    // }
}
