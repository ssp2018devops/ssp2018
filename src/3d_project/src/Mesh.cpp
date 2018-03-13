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
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "ShaderProgram.hpp"
////////////////////////////////////////////////

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
Mesh::Mesh(Vertex* vertices, size_t numVertices, GLuint texture, GLuint normalTextureId)
: M_NUM_VERTICES(numVertices)
, mTexture(texture)
, mNormalMapId(normalTextureId)
{
	// Vertex Array Object (VAO)
	glGenVertexArrays(1, &mVao);
	// bind == enable
	glBindVertexArray(mVao);
	// this activates the first and second attributes of this VAO



	// create a vertex buffer object (VBO) id
	glGenBuffers(1, &mVbo);
	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	// This "could" imply copying to the GPU, depending on what the driver wants to do...
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * M_NUM_VERTICES, vertices, GL_STATIC_DRAW);

    GLint shaderProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);
	// query where which slot corresponds to the input vertex_position in the Vertex Shader
	GLuint vertexPos = glGetAttribLocation(shaderProgram, "vertex_position");
	// specify that: the vertex attribute "vertexPos", of 3 elements of type FLOAT, not normalized, with STRIDE != 0,
	//               starts at offset 0 of the gVertexBuffer (it is implicitly bound!)

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// query where which slot corresponds to the input vertex_color in the Vertex Shader
	GLuint vertexColor = glGetAttribLocation(shaderProgram, "vertex_texCoord");
	// specify that: the vertex attribute "vertex_color", of 3 elements of type FLOAT, not normalized, with STRIDE != 0,
	//               starts at offset (12 bytes) of the gVertexBuffer

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(vertexColor, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

	glUniform1i(glGetUniformLocation(shaderProgram, "normalSampler"), 1);

	computeBoundingSphere(vertices, numVertices);
}

Mesh::Mesh(GLuint vertexArrayObject, size_t numVertices, GLuint textureId, GLuint normalTextureId)
: mVao(vertexArrayObject)
, M_NUM_VERTICES(numVertices)
, mTexture(textureId)
, mNormalMapId(normalTextureId)
{

}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
}

void Mesh::draw(GLuint mvpHandle, glm::mat4 transform) const
{
	glBindVertexArray(mVao);

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mNormalMapId);


    glm::mat4 worldTransform = getWorldTransform();

	transform = transform * worldTransform;
	glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &transform[0][0]);


    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::getCurrentProgramId(), "modelTransform"), 1, GL_FALSE, &worldTransform[0][0]);



	// draw 3 vertices starting from index 0 in the vertex array currently bound (VAO), with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, M_NUM_VERTICES);

/*
	GLint shaderProgram = ShaderProgram::getCurrentProgramId();
	glUseProgram(0);
    glm::tvec4<float> boundingSphereCenter = transform * glm::tvec4<float>(mBoundingSphereCenter.x, mBoundingSphereCenter.y, mBoundingSphereCenter.z, 1.f);
    glm::tvec4<float> boundingSphereSurfacePoint = transform * (boundingSphereCenter + glm::tvec4<float>(mBoundingSphereRadius, 0, 0, 0));


    glBegin(GL_LINES);
        glColor3b(255.f, 255.f, 255.f);
        //glVertex3f(0.f, 0.f, 0.f);
        //glVertex3f(10.f, 0.f, 0.f);
        glVertex3f(boundingSphereCenter.x, boundingSphereCenter.y, boundingSphereCenter.z); // origin of the SECOND line
        glVertex3f(boundingSphereSurfacePoint.x, boundingSphereSurfacePoint.y, boundingSphereSurfacePoint.z); // ending point of the SECOND line
    glEnd();

    glUseProgram(shaderProgram);
    */
}

GLuint Mesh::getVao() const
{
    return mVao;
}

void Mesh::computeBoundingSphere(Vertex* vertices, size_t numVertices)
{
    findBoundingSphereCenter(vertices, numVertices);
    findBoundingSphereRadius(vertices, numVertices);
}

void Mesh::findBoundingSphereCenter(Vertex* vertices, size_t numVertices)
{
    glm::vec3 sum;
    for(size_t i = 0; i < numVertices; i++)
    {
        sum.x += vertices[i].x;
        sum.y += vertices[i].y;
        sum.z += vertices[i].z;
    }

    mBoundingSphereCenter = sum / (float)numVertices;
}

void Mesh::findBoundingSphereRadius(Vertex* vertices, size_t numVertices)
{
    float maxDistanceSqrd = 0.f;
    for(size_t i = 0; i < numVertices; i++)
    {
        glm::vec3 centerToVertex = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z) - mBoundingSphereCenter;

        float distanceSqrd = glm::dot(centerToVertex, centerToVertex);
        if(distanceSqrd > maxDistanceSqrd)
            maxDistanceSqrd = distanceSqrd;
    }

    mBoundingSphereRadius = sqrtf(maxDistanceSqrd);
}

glm::vec3 Mesh::getBoundingSphereCenter() const
{
    return glm::vec3(getWorldTransform() * glm::tvec4<float>(mBoundingSphereCenter, 1.f));
}

float Mesh::getBoundingSphereRadius() const
{
    return mBoundingSphereRadius;
}
