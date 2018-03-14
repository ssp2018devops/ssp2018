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
#include "demos/ParticleSystemDemo.hpp"
#include "Camera.hpp"
#include "imageUtility.hpp"
#include "Vertex.hpp"
////////////////////////////////////////////////



////////////////////////////////////////////////
// OpenGL
#include "GL/glew.h"
//#define GL_GLEXT_PROTOTYPES
//#include "GL/gl.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "SDL2/SDL_image.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <cstdlib>
////////////////////////////////////////////////


ParticleSystemDemo::ParticleSystemDemo(const Camera& view)
: mParticleSystemProgram(ShaderProgram::Id::PARTICLE_SYSTEM)
, mView(view)
{
    loadParticleSystem("assets/textures/OOOOH_CAAAANADAAAAA.png");
}


void ParticleSystemDemo::handleInput(const SDL_Event& event)
{

}


void ParticleSystemDemo::update()
{

}


void ParticleSystemDemo::drawGeometry(glm::mat4 transform) const
{
    mParticleSystemProgram.use();
    GLint mvpHandle = ShaderProgram::getCurrentProgramUniformLocation("MVP");
    mParticleSystem->draw(mvpHandle, transform, mView);
}

void ParticleSystemDemo::loadParticleSystem(std::string texturePath)
{
    mParticleSystemProgram.use();

    const int NUM_PARTICLE_COLUMNS = 100;
    const int NUM_PARTICLE_ROWS = 100;
    Vertex particles[NUM_PARTICLE_COLUMNS * NUM_PARTICLE_ROWS];
    const float SCALE = 0.1f;

    glm::vec3 position(-NUM_PARTICLE_COLUMNS / 2, -NUM_PARTICLE_ROWS / 2, 0.f);
    position *= SCALE;
    for(size_t y = 0, i = 0; y < NUM_PARTICLE_ROWS; y++)
    {
        float positionX = position.x;
        for(size_t x = 0; x < NUM_PARTICLE_COLUMNS; x++)
        {
            particles[i] = {positionX, position.y, (float)(rand() % 200) / 2.f - 1.f};

            positionX += 1.f * SCALE;
            i++;
        }

        position.y += 1.f * SCALE;
    }

    GLuint particleTextureId;
    glGenTextures(1, &particleTextureId);
    glBindTexture(GL_TEXTURE_2D, particleTextureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    Image particleTexture = readImage(texturePath);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, particleTexture.data->w, particleTexture.data->h, 0, GL_RGBA,  GL_UNSIGNED_BYTE, particleTexture.data->pixels);
    mParticleSystem = std::unique_ptr<ParticleSystem>(new ParticleSystem(particles, NUM_PARTICLE_COLUMNS * NUM_PARTICLE_ROWS, particleTextureId));
}

