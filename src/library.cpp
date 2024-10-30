#include "library.hpp"


void Library::add(const std::string& filePath, int img_width, int img_height)
{
    printInfo(TextFormat("Loading texture: %s", filePath.c_str()));
    if (Utils::isImage(filePath)) {
        items.push_back(Item{
            .gif   = Gif(""),
            .isGif = false,
            .size  = {static_cast<float>(img_width), static_cast<float>(img_height)},
            .path  = filePath
        });
    }
    else if (filePath.ends_with(".gif")) {
        items.push_back(Item{
            .gif   = Gif(filePath.c_str()),
            .isGif = true,
            .size  = {static_cast<float>(img_width), static_cast<float>(img_height)},
            .path  = filePath
        });
    }

    ++librarySize;
}


void Library::add(const std::string& filePath, const Image& image)
{
    printInfo(TextFormat("Loading texture: %s", filePath.c_str()));
    if (Utils::isImage(filePath)) {
        items.push_back(Item{
            .gif   = Gif(""),
            .isGif = false,
            .size  = {static_cast<float>(image.width), static_cast<float>(image.height)},
            .path  = filePath
        });
    }
    else if (filePath.ends_with(".gif")) {
        items.push_back(Item{
            .gif   = Gif(filePath.c_str()),
            .isGif = true,
            .size  = {static_cast<float>(image.width), static_cast<float>(image.height)},
            .path  = filePath
        });
    }

    ++librarySize;
}


void Library::update(const float& frameTime)
{
    for (Item& item : items) {
        if (item.isGif && item.isLoaded) {
            // Update gif frame
            item.gif.update(frameTime, item.texture);
        }
    }
}


Library::~Library()
{
    for (Item& item : items) {
        if (item.isLoaded) {
            UnloadTexture(item.texture);
            printInfo(TextFormat("Unloaded texture: %s", item.path.c_str()));
        }

        if (item.isGif) {
            UnloadImage(item.gif.img);
            printInfo(TextFormat("Unloaded gif img: %s", item.path.c_str()));
        }
    }
}
