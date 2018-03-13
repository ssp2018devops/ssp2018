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

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

////////////////////////////////////////////////
// Internal headers
#include "Window.hpp"
#include "World.hpp"
////////////////////////////////////////////////

class Application
{
    public:
        Application();

        void run();

    private:
        void update();


    private:
        Window mWindow;
        World mWorld;
};
#endif // APPLICATION_HPP

