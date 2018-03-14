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

#ifndef REFLECTION_MESH_HPP
#define REFLECTION_MESH_HPP

////////////////////////////////////////////////
// Internal headers
#include "Mesh.hpp"
#include "Camera.hpp"
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


class ReflectionMesh : public Mesh
{
    public:
        ReflectionMesh(Vertex* vertices, size_t numVertices, GLuint texture, GLuint normalTextureId);

		void draw(GLuint mvpHandle, glm::mat4 transform) const;
        glm::mat4 getCameraTransform(size_t index) const;



    private:
        std::vector<Camera> mCameras;
};







#endif // REFLECTION_MESH_HPP
