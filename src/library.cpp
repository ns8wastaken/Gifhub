#include "library.hpp"


void Library::add(const std::string& fileName)
{
    Image image = LoadImage(fileName.c_str());
    Utils::ClampImageSize(&image);

    if (fileName.ends_with(".gif")) {
        m_items.push_back(Item{
            .media   = {.gif = Gif(fileName.c_str())},
            .isGif   = true,
            .size    = {static_cast<float>(image.width), static_cast<float>(image.height)},
            .texture = LoadTextureFromImage(image),
            .id      = m_librarySize
        });
    }
    else {
        m_items.push_back(Item{
            .media   = {.img = image},
            .isGif   = false,
            .size    = {static_cast<float>(image.width), static_cast<float>(image.height)},
            .texture = LoadTextureFromImage(image),
            .id      = m_librarySize
        });
    }

    UnloadImage(image);
    ++m_librarySize;
}


void Library::update(const float& frameTime)
{
    for (Item& item : m_items) {
        // Update gif frame
        if (item.isGif) {
            item.media.gif.update(frameTime, item.texture);
        }
    }
}


const std::vector<Library::Item>& Library::getItems()
{
    return m_items;
}


Library::~Library()
{
    // for (Item& item : m_items) {
    //     if (!item.isGif) UnloadTexture(item.media.img);
    // }
}
