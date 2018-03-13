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

#ifndef TERRAIN_DEMO_HPP
#define TERRAIN_DEMO_HPP

////////////////////////////////////////////////
// Internal headers
#include "Demo.hpp"
#include "ShaderProgram.hpp"
#include "Terrain.hpp"
#include "Camera.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <list>
#include <memory>
////////////////////////////////////////////////



class TerrainDemo : public Demo
{
    public:
        TerrainDemo(Camera& playerCam);

        void handleInput(const SDL_Event& event);
        void update();
        void drawGeometry(glm::mat4 transform) const;

    private:
        ShaderProgram mTerrainProgram;
        std::list<std::unique_ptr<Terrain>> mTerrain;
        Camera& mCamera;
        bool mIsPlayerWalking = true;
};

#endif // TERRAIN_DEMO_HPP
