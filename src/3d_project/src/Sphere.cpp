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
#include "Sphere.hpp"
#include "Vertex.hpp"
#include "ShaderProgram.hpp"
////////////////////////////////////////////////



#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
Sphere::Sphere(Vertex center, float radius)
{

	// Vertex Array Object (VAO)
	glGenVertexArrays(1, &mVao);
	// bind == enable
	glBindVertexArray(mVao);
	// this activates the first and second attributes of this VAO
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// create a vertex buffer object (VBO) id
	glGenBuffers(1, &mVbo);
	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	// This "could" imply copying to the GPU, depending on what the driver wants to do...
	center.u = radius;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * M_NUM_VERTICES, &center, GL_STATIC_DRAW);

    GLint shaderProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);
	// query where which slot corresponds to the input vertex_position in the Vertex Shader
	GLint vertexPos = glGetAttribLocation(shaderProgram, "vertex_position");
	// specify that: the vertex attribute "vertexPos", of 3 elements of type FLOAT, not normalized, with STRIDE != 0,
	//               starts at offset 0 of the gVertexBuffer (it is implicitly bound!)
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));

	// query where which slot corresponds to the input vertex_color in the Vertex Shader
	GLint radiusHandle = glGetAttribLocation(shaderProgram, "sphere_radius");
	// specify that: the vertex attribute "vertex_color", of 3 elements of type FLOAT, not normalized, with STRIDE != 0,
	//               starts at offset (12 bytes) of the gVertexBuffer
	glVertexAttribPointer(radiusHandle, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));
}

Sphere::~Sphere()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
}

void Sphere::draw(GLuint mvpHandle, glm::mat4 transform) //const
{
	glBindVertexArray(mVao);

    glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &transform[0][0]);

	glDrawArrays(GL_POINTS, 0, M_NUM_VERTICES);
}
