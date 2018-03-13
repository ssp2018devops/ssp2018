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

#ifndef IMAGE_UTILITY_HPP
#define IMAGE_UTILITY_HPP

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <string>
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "SDL2/SDL_image.h"
////////////////////////////////////////////////

struct Image
{
    Image(SDL_Surface* data);
    ~Image();
    Image(const Image&);
    Image& operator=(const Image&);

    void free();

    SDL_Surface* data = nullptr;
};

Image readImage(std::string filePath);
unsigned char*  getPixels(const Image& image);


#endif // IMAGE_UTILITY_HPP
