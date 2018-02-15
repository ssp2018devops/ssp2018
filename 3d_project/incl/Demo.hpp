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

#ifndef DEMO_HPP
#define DEMO_HPP

////////////////////////////////////////////////
// OpenGL Mathematics (GLM)
#include "glm/mat4x4.hpp"
////////////////////////////////////////////////

union SDL_Event;

class Demo
{
    public:
        virtual void handleInput(const SDL_Event& event);
        virtual void update();
        virtual void drawGeometry(glm::mat4 transform) const;
        virtual void drawLight();
};

#endif // DEMO_HPP
