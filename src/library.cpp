#include "library.hpp"


void Library::addGif(const char* fileName, const float gifDelay = 42.0f)
{
    ++librarySize;
    gifs.push_back(Gif(fileName, gifDelay));
}


void Library::addImage(const char* fileName)
{
    ++librarySize;

    Image image = LoadImage(fileName);
    imgs.push_back(LoadTextureFromImage(image));
    UnloadImage(image);
}


void Library::update(const float& frameTime)
{
    for (Gif& gif : gifs) {
        gif.update(frameTime);
    }
}


const std::vector<Texture>& Library::getImages()
{
    return imgs;
}


const std::vector<Gif>& Library::getGifs()
{
    return gifs;
}
