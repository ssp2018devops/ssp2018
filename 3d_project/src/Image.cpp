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
#include "Image.hpp"
#include "Vertex.hpp"
////////////////////////////////////////////////



#include "glext/glext.h"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
Mesh::Mesh(Vertex* vertices, size_t numVertices)
: M_NUM_VERTICES(numVertices)
//, mVertices(new Vertex[3]())
{
    //for(int i = 0; i < 3; i++)
    //    mVertices[i] = vertices[i];




    /*
    derp = new Derp[numVertices];
    for(int i = 0; i < numVertices; i++)
    {
        derp[i].x = vertices[i].x;
        derp[i].y = vertices[i].y;
        derp[i].z = vertices[i].z;
        //derp[i].u = vertices[i].u;
        //derp[i].v = vertices[i].v;

        std::cout << derp[i].x << " " << derp[i].y << " " << derp[i].z << std::endl;
    }
    */

	// Model matrix : an identity matrix (model will be at the origin)
	mWorldMatrix = glm::mat4(1.0f);

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

    GLint shaderProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);
	// query where which slot corresponds to the input vertex_position in the Vertex Shader
	GLuint vertexPos = glGetAttribLocation(shaderProgram, "vertex_position");
	// specify that: the vertex attribute "vertexPos", of 3 elements of type FLOAT, not normalized, with STRIDE != 0,
	//               starts at offset 0 of the gVertexBuffer (it is implicitly bound!)
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));

	// query where which slot corresponds to the input vertex_color in the Vertex Shader
	//GLuint vertexColor = glGetAttribLocation(shaderProgram, "vertex_texCoord");
	// specify that: the vertex attribute "vertex_color", of 3 elements of type FLOAT, not normalized, with STRIDE != 0,
	//               starts at offset (12 bytes) of the gVertexBuffer
	//glVertexAttribPointer(vertexColor, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 4));
	//glVertexAttribPointer(vertexColor, 2, GL_FLOAT, GL_FALSE, sizeof(Derp), BUFFER_OFFSET(sizeof(float) * 3));


}

void Mesh::draw(GLuint mvpHandle, glm::mat4 transform) const
{
	glBindVertexArray(mVao);

	transform *= mWorldMatrix;
	glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &transform[0][0]);

	// draw 3 vertices starting from index 0 in the vertex array currently bound (VAO), with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, M_NUM_VERTICES);
}
