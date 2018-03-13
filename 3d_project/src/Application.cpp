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
#include "Application.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <chrono>
#include <iostream>
////////////////////////////////////////////////

Application::Application()
: mWindow(640, 640)
, mWorld(mWindow)
{

}

void Application::run()
{
    using namespace std::chrono;
    milliseconds timeSinceLastUpdate(0);
    time_point<high_resolution_clock> lastTime = high_resolution_clock::now();
    milliseconds msPerFrame(int((1.f / 60.f) * 1000.f));
    size_t numFrames = 0;
    while(mWindow.isOpen())
    {
        milliseconds deltaTime = duration_cast<milliseconds>(high_resolution_clock::now() - lastTime);
        if(deltaTime < msPerFrame)
        {
            milliseconds surplusTime = msPerFrame - deltaTime;
            SDL_Delay(surplusTime.count());
            deltaTime += surplusTime;
        }

        timeSinceLastUpdate += deltaTime;
        lastTime = high_resolution_clock::now();

        while(timeSinceLastUpdate > msPerFrame)
        {
            timeSinceLastUpdate -= msPerFrame;
            update();
        }

        mWorld.draw();
        mWindow.draw();

        numFrames++;
//        std::cout << 1.f / (((float)deltaTime.count()) / 1000.f) << " fps" << std::endl;
    }
}

void Application::update()
{
    mWorld.handleInput();
    mWorld.update();
    mWindow.update();
}
