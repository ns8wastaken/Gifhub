#include "library.hpp"


// void Library::add(const std::string& fileName)
// {
//     Image image = LoadImage(fileName.c_str());
//     Utils::ClampImageSize(&image);

//     if (fileName.ends_with(".gif")) {
//         items.push_back(Item{
//             .gif   = Gif(""),
//             .isGif = true,
//             .size  = {static_cast<float>(image.width), static_cast<float>(image.height)},
//             .id    = librarySize,
//             .path  = fileName
//         });
//     }
//     else {
//         items.push_back(Item{
//             .gif   = Gif(fileName.c_str()),
//             .isGif = false,
//             .size  = {static_cast<float>(image.width), static_cast<float>(image.height)},
//             .id    = librarySize,
//             .path  = fileName
//         });
//     }

//     UnloadImage(image);
//     ++librarySize;
// }


void Library::add(const std::string& fileName, const Image& image)
{
    printInfo(fileName.c_str());
    if (Utils::isImage(fileName)) {
        items.push_back(Item{
            .gif   = Gif(""),
            .isGif = false,
            .size  = {static_cast<float>(image.width), static_cast<float>(image.height)},
            .id    = librarySize,
            .path  = fileName
        });
    }
    else if (fileName.ends_with(".gif")) {
        items.push_back(Item{
            .gif   = Gif(fileName.c_str()),
            .isGif = true,
            .size  = {static_cast<float>(image.width), static_cast<float>(image.height)},
            .id    = librarySize,
            .path  = fileName
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
