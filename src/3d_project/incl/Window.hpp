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

#ifndef WINDOW_HPP
#define WINDOW_HPP

////////////////////////////////////////////////
// Internal headers
#include "Camera.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
////////////////////////////////////////////////

class Window
{
    public:
        Window(size_t width, size_t height);
        ~Window();

        bool isOpen() const;

        void draw() const;
        void update();

        void moveView(float x, float y, float z);
        void rotateView(float radiansX, float radiansY);

        void close();

         Camera& getView();

        size_t getWidth() const;
        size_t getHeight() const;

    private:
        SDL_Window* mWindow;
        SDL_GLContext mGlContext;
        Camera mCamera;
};


#endif // WINDOW_HPP
