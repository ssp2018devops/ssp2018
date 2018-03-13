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

#ifndef DYN_CUBE_ENV_MAPPING_DEMO_HPP
#define DYN_CUBE_ENV_MAPPING_DEMO_HPP

////////////////////////////////////////////////
// Internal headers
#include "Demo.hpp"
#include "Framebuffer.hpp"
#include "ShaderProgram.hpp"
#include "Mesh.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <list>
#include <memory>
////////////////////////////////////////////////



class DynCubeEnvMappingDemo : public Demo
{
    public:
        DynCubeEnvMappingDemo(const Framebuffer& deferredBuffer, const ShaderProgram& lightPassProgram);

        void handleInput(const SDL_Event& event);
        void update();
        void drawGeometry(glm::mat4 transform) const;

    private:
        void loadMeshes(size_t numMeshes, std::string objName);

        void draw(glm::mat4 transform) const;


    private:
        ShaderProgram mMeshProgram;
        ShaderProgram mCubeMapProgram;
        const ShaderProgram& mLightPassProgram;
        const Framebuffer& mDeferredBuffer;
        Framebuffer mCubeMapBuffer;

        const size_t M_NUM_MESHES;
        std::list<std::unique_ptr<Mesh>> mMeshes;

};

#endif // DYN_CUBE_ENV_MAPPING_DEMO_HPP
