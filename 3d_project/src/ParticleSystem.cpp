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
#include "ParticleSystem.hpp"
#include "Vertex.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
////////////////////////////////////////////////




#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <cassert>
ParticleSystem::ParticleSystem(Vertex* vertices, size_t numVertices, GLuint textureId)
: M_NUM_VERTICES(numVertices)
, mTextureId(textureId)
{
	// Model matrix : an identity matrix (model will be at the origin)
	mWorldMatrix = glm::mat4(1.0f);
//    mWorldMatrix = glm::translate(mWorldMatrix, glm::vec3(1,0,0));

	// Vertex Array Object (VAO)
	glGenVertexArrays(1, &mVao);
	// bind == enable
	glBindVertexArray(mVao);
	// this activates the first and second attributes of this VAO

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	// create a vertex buffer object (VBO) id
	glGenBuffers(1, &mVbo);
	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	// This "could" imply copying to the GPU, depending on what the driver wants to do...
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * M_NUM_VERTICES, vertices, GL_STATIC_DRAW);

    GLint shaderProgram = ShaderProgram::getCurrentProgramId();
	// query where which slot corresponds to the input vertex_position in the Vertex Shader
	GLuint vertexPos = glGetAttribLocation(shaderProgram, "vertex_position");
	// specify that: the vertex attribute "vertexPos", of 3 elements of type FLOAT, not normalized, with STRIDE != 0,
	//               starts at offset 0 of the gVertexBuffer (it is implicitly bound!)
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
}

ParticleSystem::~ParticleSystem()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
}

void ParticleSystem::draw(GLuint mvpHandle, glm::mat4 transform, const Camera& camera) //const
{
	glBindVertexArray(mVao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

	transform = transform * mWorldMatrix;
	glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &transform[0][0]);

    glm::vec3 cameraForward;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    camera.getDirectionVectors(cameraForward, cameraUp, cameraRight);

    glUniform3fv(glGetUniformLocation(ShaderProgram::getCurrentProgramId(), "cameraUp"), 1, &cameraUp[0]);
    glUniform3fv(glGetUniformLocation(ShaderProgram::getCurrentProgramId(), "cameraRight"), 1, &cameraRight[0]);

	// draw 3 vertices starting from index 0 in the vertex array currently bound (VAO), with current in-use shader
	glDrawArrays(GL_POINTS, 0, M_NUM_VERTICES);
}
