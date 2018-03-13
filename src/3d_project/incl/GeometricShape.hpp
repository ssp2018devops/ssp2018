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

#ifndef GEOMETRIC_SHAPE_HPP
#define GEOMETRIC_SHAPE_HPP

////////////////////////////////////////////////
// Internal headers
class Vertex;
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
////////////////////////////////////////////////


class GeometricShape
{
    public:
        GeometricShape();

        void move(glm::vec3 v);
        void rotate(float radians, glm::vec3 axis);

        glm::mat4 getWorldTransform() const;
        glm::mat4 getTranslation() const;

    private:
        glm::mat4 mTranslation;
		glm::mat4 mRotation;
};

#endif // GEOMETRIC_SHAPE_HPP
