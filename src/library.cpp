#include "library.hpp"


void Library::addGif(const char* fileName, const float gifDelay)
{
    ++librarySize;
    gifs.push_back(Gif(fileName, gifDelay));
}


void Library::addImage(const char* fileName)
{
    ++librarySize;
    imgs.push_back(LoadImage(fileName));
}


void Library::update(const float& frameTime)
{
    for (Gif& gif : gifs) {
        gif.update(frameTime);
    }
}
