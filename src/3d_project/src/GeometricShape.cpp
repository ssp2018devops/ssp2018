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
#include "GeometricShape.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "glm/gtc/matrix_transform.hpp"
////////////////////////////////////////////////

GeometricShape::GeometricShape()
: mTranslation(1.f)
, mRotation(1.f)
{

}

void GeometricShape::move(glm::vec3 v)
{
    mTranslation = glm::translate(mTranslation, v);
}

void GeometricShape::rotate(float radians, glm::vec3 axis)
{
    mRotation = glm::rotate(mRotation, radians, axis);
}

glm::mat4 GeometricShape::getWorldTransform() const
{
    return mRotation * mTranslation;
}

glm::mat4 GeometricShape::getTranslation() const
{
    return mTranslation;
}
