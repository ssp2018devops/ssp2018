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

#ifndef PARTICLE_SYSTEM_DEMO_HPP
#define PARTICLE_SYSTEM_DEMO_HPP

////////////////////////////////////////////////
// Internal headers
#include "Demo.hpp"
#include "ShaderProgram.hpp"
#include "ParticleSystem.hpp"
class Camera;
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <memory>
////////////////////////////////////////////////



class ParticleSystemDemo : public Demo
{
    public:
        ParticleSystemDemo(const Camera& view);

        void handleInput(const SDL_Event& event);
        void update();
        void drawGeometry(glm::mat4 transform) const;

    private:
        void loadParticleSystem(std::string texturePath);

    private:
        ShaderProgram mParticleSystemProgram;
        const Camera& mView;
        std::unique_ptr<ParticleSystem> mParticleSystem;
};

#endif // PARTICLE_SYSTEM_DEMO_HPP
