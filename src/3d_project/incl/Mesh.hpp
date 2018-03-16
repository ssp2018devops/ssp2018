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

#ifndef MESH_HPP
#define MESH_HPP

////////////////////////////////////////////////
// Internal headers
struct Vertex;
#include "GeometricShape.hpp"
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


class Mesh : public GeometricShape
{
    public:
		Mesh(Vertex* vertices, size_t numVertices, GLuint texture, GLuint normalTextureId);
		Mesh(GLuint vertexArrayObject, size_t numVertices, GLuint textureId, GLuint normalTextureId);
		~Mesh();
		void draw(GLuint mvpHandle, glm::mat4 transform) const;

        glm::vec3 getBoundingSphereCenter() const;
        float getBoundingSphereRadius() const;

        GLuint getVao() const;

    private:
        void computeBoundingSphere(Vertex* vertices, size_t numVertices);
        void findBoundingSphereCenter(Vertex* vertices, size_t numVertices);
        void findBoundingSphereRadius(Vertex* vertices, size_t numVertices);

    protected:
        const size_t M_NUM_VERTICES;

        GLuint mVao;
		GLuint mVbo;
		GLuint mTexture;
        GLuint mNormalMapId;

        glm::vec3 mBoundingSphereCenter;
		float mBoundingSphereRadius;


};







#endif // MESH_HPP
