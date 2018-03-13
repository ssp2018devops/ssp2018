/****************************************************************
****************************************************************
*
* {{PROJECT_NAME}}
* {{PROJECT_DESCRIPTION}}
* Mikael Hernvall (mikael@hernvall.com)
* 2016
*
****************************************************************
****************************************************************/

////////////////////////////////////////////////
// Internal headers
#include "imageUtility.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <stdexcept>
////////////////////////////////////////////////

void Image::free()
{
    SDL_FreeSurface(data);
    data = nullptr;
}

Image::Image(SDL_Surface* data)
: data(data)
{

}

Image::~Image()
{
    free();
}

Image::Image(const Image& other)
{
    if(&other != this)
        free();

    data = other.data;
}

Image& Image::operator=(const Image& other)
{
    if(&other != this)
        free();

    data = other.data;

    return *this;
}
#include <iostream>
Image readImage(std::string filePath)
{
    SDL_Surface* image = IMG_Load(filePath.c_str());
    if(!image)
    {
        printf("IMG_Load: %s\n", IMG_GetError());
        throw std::runtime_error("Failed to load image at \"" + filePath + "\".");
    }

    return {image};
}

unsigned char* getPixels(const Image& image)
{
    const size_t NUM_PIXELS_PER_ROW = image.data->w;
    const size_t IMAGE_HEIGHT = image.data->h;
    const size_t NUM_PIXELS = NUM_PIXELS_PER_ROW * IMAGE_HEIGHT;

    unsigned char* pixels = new unsigned char[NUM_PIXELS];

    const size_t ROW_SIZE = image.data->pitch;
    const size_t IMAGE_SIZE = IMAGE_HEIGHT * ROW_SIZE;

    // Simply remove row padding.
    for(size_t iRowSrc = 0, iRowDest = 0; iRowSrc < IMAGE_SIZE; iRowSrc += ROW_SIZE, iRowDest += NUM_PIXELS_PER_ROW)
        memcpy(pixels + iRowDest, (unsigned char*)image.data->pixels + iRowSrc, NUM_PIXELS_PER_ROW);

    return pixels;
}
