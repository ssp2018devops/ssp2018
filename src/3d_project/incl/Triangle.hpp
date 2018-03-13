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

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "glm/mat4x4.hpp"
////////////////////////////////////////////////

#include "Vertex.hpp"

//////////////////////////////////////////////////////////////////
// TRIANGLE
//////////////////////////////////////////////////////////////////
class Triangle
{
	public:
		Triangle(Vertex v1, Vertex v2, Vertex v3);

		void draw(GLuint mvpHandle, glm::mat4 transform) const;

		void rotate(float radians);

	private:
		static const unsigned int M_NUM_VERTICES = 3;
		Vertex mVertices[M_NUM_VERTICES];

		unsigned int mVao;
		unsigned int mVbo;

		glm::mat4 mWorldMatrix;
};

#endif // TRIANGLE_HPP
