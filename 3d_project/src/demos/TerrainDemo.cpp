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
#include "demos/TerrainDemo.hpp"
////////////////////////////////////////////////

#include "SDL2/SDL_events.h"


TerrainDemo::TerrainDemo(Camera& playerCam)
: mTerrainProgram(ShaderProgram::Id::TERRAIN)
, mCamera(playerCam)
{
    const std::string texturesDir = "assets/textures/";
    mTerrainProgram.use();
    mTerrain.push_back(std::unique_ptr<Terrain>(new Terrain(texturesDir + "es1_heightmap.png", texturesDir + "es1_texture.png")));
}


void TerrainDemo::handleInput(const SDL_Event& event)
{
    switch(event.type)
    {
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_LCTRL)
                mIsPlayerWalking = !mIsPlayerWalking;
            break;
        default:
            break;
    }
}

#include <iostream>
void TerrainDemo::update()
{
    if(mIsPlayerWalking)
    {
        glm::vec3 cameraPosition = mCamera.getPosition();
        float height = mTerrain.front()->getHeight(cameraPosition.x, cameraPosition.z);
        std::cout << "camY: " << cameraPosition.y << std::endl;
        mCamera.move(0.f, (height + 2.f) - cameraPosition.y, 0.f);
    }


//    for(auto& terrain : mTerrain)
//        terrain->rotate(0.003f, glm::vec3(0.f, 1.f, 0.f));
}


void TerrainDemo::drawGeometry(glm::mat4 transform) const
{
    mTerrainProgram.use();
    GLint mvpHandle = ShaderProgram::getCurrentProgramUniformLocation("MVP");
    for(auto& terrain : mTerrain)
        terrain->draw(mvpHandle, transform);
}
