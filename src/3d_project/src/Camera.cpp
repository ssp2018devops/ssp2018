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
#include "Camera.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <iostream>
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL Mathematics (GLM)
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
////////////////////////////////////////////////

Camera::Camera(float fieldOfViewRadians, float aspectRatio, float nearDistance, float farDistance, glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camUp)
: mPosition(0.f, 0.f, 0.f)
, mRotation(1.f)
, mFrustumCenter(camForward * (nearDistance + (farDistance - nearDistance) / 2.f), 1.f)
{

	mProjection = glm::perspective
	(
        fieldOfViewRadians,
        aspectRatio,
        nearDistance,
        farDistance
    );

    mPosition = -camPos;
	mView = glm::lookAt
    (
        glm::vec3(0, 0, 0),
        camForward,
        camUp
    );
}

void Camera::move(float x, float y, float z)
{
    mPosition -= glm::vec3(glm::vec4(x, 0, z, 0.f) * mRotation);
    mPosition.y -= y;
}

void Camera::rotate(float radiansX, float radiansY, float radiansZ)
{

    if(mRotationY + radiansY > HALF_PI)
        radiansY = HALF_PI - mRotationY;
    else if(mRotationY + radiansY < -HALF_PI)
        radiansY = -HALF_PI - mRotationY;

    mRotationY += radiansY;


    mRotation = glm::rotate(mRotation, radiansX, glm::vec3(0.f, 1.f, 0.f));
    mRotation = glm::rotate(mRotation, radiansY, glm::vec3(glm::vec4(1.f, 0.f, 0.f, 0.f) * mRotation));
    mRotation = glm::rotate(mRotation, radiansZ, glm::cross(glm::vec3(glm::vec4(1.f, 0.f, 0.f, 0.f) * mRotation), glm::vec3(glm::vec4(0.f, 1.f, 0.f, 0.f) * mRotation)));
}

glm::mat4 Camera::getGlobalTransform() const
{
    return mProjection * mRotation * glm::translate(mView, mPosition);
}

glm::vec3 Camera::getForward() const
{
    return glm::vec3(-mView[2] * mRotation);
}

void Camera::getDirectionVectors(glm::vec3& forward, glm::vec3& up, glm::vec3& right) const
{
    glm::mat4 view = glm::inverse(mRotation) * mView;
    forward = -glm::vec3(view[2]);
    up = glm::vec3(view[1]);
    right = glm::cross(forward, up);
}

glm::mat4 Camera::getViewTransform() const
{
    return mView;
}

glm::vec4 Camera::getLocalForward() const
{
    return -mView[2];
}

glm::vec4 Camera::getLocalUp() const
{
    return mView[1];
}

glm::mat4 Camera::getProjectionTransform() const
{
    return mProjection;
}

glm::vec3 Camera::getPosition() const
{
    return glm::vec3(getWorldTransform() * glm::vec4(0.f, 0.f, 0.f, 1.f));
}

glm::mat4 Camera::getWorldTransform() const
{
    return glm::inverse(mRotation * glm::translate(glm::mat4(1.f), mPosition));
}

std::vector<glm::vec4> Camera::getFrustomCorners() const
{
    const size_t NUM_VERTICES = 8;
    std::vector<glm::vec4> vertices =
    {
        glm::vec4(-1.f, 1.f, 1.f, 1.f),
        glm::vec4(-1.f, 1.f, -1.f, 1.f),
        glm::vec4(1.f, 1.f, -1.f, 1.f),
        glm::vec4(1.f, 1.f, 1.f, 1.f),

        glm::vec4(-1.f, -1.f, 1.f, 1.f),
        glm::vec4(-1.f, -1.f, -1.f, 1.f),
        glm::vec4(1.f, -1.f, -1.f, 1.f),
        glm::vec4(1.f, -1.f, 1.f, 1.f)
    };

    glm::mat4 inverseProjection = glm::inverse(mProjection);
    for(size_t i = 0; i < NUM_VERTICES; i++)
    {
        vertices[i] = inverseProjection * vertices[i];
        vertices[i] /= vertices[i].w;
    }

    return vertices;
}

glm::vec4 Camera::getFrustumCenter() const
{
    return mFrustumCenter;
}

