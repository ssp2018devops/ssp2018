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
#include "ReflectionMesh.hpp"
#include "ShaderProgram.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
////////////////////////////////////////////////

ReflectionMesh::ReflectionMesh(Vertex* vertices, size_t numVertices, GLuint texture, GLuint normalTextureId)
: Mesh(vertices, numVertices, texture, normalTextureId)
, mCameras
({
    Camera(glm::pi<float>() / 2.f, 1.f, 0.1f, 2000.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)),
    Camera(glm::pi<float>() / 2.f, 1.f, 0.1f, 2000.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)),
    Camera(glm::pi<float>() / 2.f, 1.f, 0.1f, 2000.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)),
    Camera(glm::pi<float>() / 2.f, 1.f, 0.1f, 2000.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -1.f)),
    Camera(glm::pi<float>() / 2.f, 1.f, 0.1f, 2000.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f)),
    Camera(glm::pi<float>() / 2.f, 1.f, 0.1f, 2000.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, -1.f, 0.f)),
})
{

}

glm::mat4 ReflectionMesh::getCameraTransform(size_t index) const
{
    if(index >= mCameras.size())
        throw std::runtime_error("Index out of bounds.");

    return glm::translate(mCameras[index].getGlobalTransform(), -glm::vec3(getWorldTransform()[3]));
}

void ReflectionMesh::draw(GLuint mvpHandle, glm::mat4 transform) const
{
	glBindVertexArray(mVao);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mNormalMapId);

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

    glm::mat4 worldTransform = getWorldTransform();

	transform = transform * worldTransform;
	glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &transform[0][0]);


    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::getCurrentProgramId(), "modelTransform"), 1, GL_FALSE, &worldTransform[0][0]);

    glm::vec3 meshCenter = getBoundingSphereCenter();
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("meshCenter"), 1, &meshCenter[0]);

//    glUniform3f(ShaderProgram::getCurrentProgramUniformLocation("meshCenter"), mBoundingSphereCenter.x, mBoundingSphereCenter.y, mBoundingSphereCenter.z);


	glDrawArrays(GL_TRIANGLES, 0, M_NUM_VERTICES);
}
