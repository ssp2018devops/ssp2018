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
#include "Window.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL
#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <iostream>
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"
////////////////////////////////////////////////

Window::Window(size_t width, size_t height)
: mCamera(glm::pi<float>() * 0.45f,  ((float)width) / ((float)height), 0.5f, 2000.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f))
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Failed to initialize SDL." << std::endl;
        throw std::runtime_error(SDL_GetError());
    }

    mWindow = SDL_CreateWindow
    (
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        width,                               // width, in pixels
        height,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );


    if(mWindow == NULL)
    {
        std::cout << "Failed to create SDL window." << std::endl;
        throw std::runtime_error(SDL_GetError());
    }


    mGlContext = SDL_GL_CreateContext(mWindow);
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    if(mGlContext == NULL)
    {
        std::cout << "Failed to create OpenGL context." << std::endl;
        throw std::runtime_error(SDL_GetError());
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glViewport(0, 0, width, height);

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

////////////////////////////////////////////////

Window::~Window()
{
    close();
}

////////////////////////////////////////////////

bool Window::isOpen() const
{
    return mWindow != NULL;
}

////////////////////////////////////////////////

void Window::draw() const
{
    SDL_GL_SwapWindow(mWindow);
}

////////////////////////////////////////////////

void Window::update()
{
    SDL_PumpEvents();
}

////////////////////////////////////////////////

void Window::close()
{
    SDL_DestroyWindow(mWindow);
    mWindow = NULL;

    SDL_GL_DeleteContext(mGlContext);

    SDL_Quit();
}

////////////////////////////////////////////////

void Window::moveView(float x, float y, float z)
{
//    mCamera.move(x * 100.f, y * 100.f, z * 100.f);
    mCamera.move(x, y, z);
}

////////////////////////////////////////////////

void Window::rotateView(float radiansX, float radiansY)
{
    mCamera.rotate(radiansX, radiansY);
}

////////////////////////////////////////////////

Camera& Window::getView()
{
    return mCamera;
}

////////////////////////////////////////////////

size_t Window::getWidth() const
{
    int width;
    SDL_GetWindowSize(mWindow, &width, nullptr);
    return width;
}

////////////////////////////////////////////////

size_t Window::getHeight() const
{
    int height;
    SDL_GetWindowSize(mWindow, nullptr, &height);
    return height;
}
