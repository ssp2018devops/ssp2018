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

#ifndef SPHERE_HPP
#define SPHERE_HPP

////////////////////////////////////////////////
// Internal headers
class Vertex;
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


class Sphere
{
    public:
		Sphere(Vertex center, float radius);
		~Sphere();
		void draw(GLuint mvpHandle, glm::mat4 transform); //const;

    private:
        const size_t M_NUM_VERTICES = 1;

        GLuint mVao;
		GLuint mVbo;
};







#endif // SPHERE_HPP
