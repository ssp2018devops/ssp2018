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

#ifndef MOUSE_PICKING_DEMO_HPP
#define MOUSE_PICKING_DEMO_HPP

////////////////////////////////////////////////
// Internal headers
#include "Demo.hpp"
#include "ShaderProgram.hpp"
#include "Mesh.hpp"
class Framebuffer;
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <memory>
#include <list>
////////////////////////////////////////////////



class MousePickingDemo : public Demo
{
    public:
        MousePickingDemo(const Framebuffer& frameBuffer);

        void handleInput(const SDL_Event& event);
        void update();
        void drawGeometry(glm::mat4 transform) const;

    private:
        void loadMeshes(size_t numMeshes, std::string objName);

    private:
        const Framebuffer& mFramebuffer;
        ShaderProgram mMeshProgram;
        const size_t M_NUM_MESHES;
        std::list<std::unique_ptr<Mesh>> mMeshes;
};

#endif // MOUSE_PICKING_DEMO_HPP
