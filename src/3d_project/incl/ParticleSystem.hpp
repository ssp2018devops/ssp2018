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

#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP


////////////////////////////////////////////////
// Internal headers
class Vertex;
class Camera;
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL
#include "GL/glew.h"
//#define GL_GLEXT_PROTOTYPES
//#include "GL/gl.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
////////////////////////////////////////////////


class ParticleSystem
{
    public:
		ParticleSystem(Vertex* vertices, size_t numVertices, GLuint textureId);
		~ParticleSystem();

		void update();
		void draw(GLuint mvpHandle, glm::mat4 transform, const Camera& camera); //const

    private:
        const size_t M_NUM_VERTICES;
        GLuint mVao;
		GLuint mVbo;

        GLuint mTextureId;

		glm::mat4 mWorldMatrix;
};


#endif // PARTICLE_SYSTEM_HPP
