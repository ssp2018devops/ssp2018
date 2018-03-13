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

#ifndef CAMERA_HPP
#define CAMERA_HPP

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <vector>
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL Mathematics (GLM)
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
////////////////////////////////////////////////

class Camera
{
    public:
        Camera(float fieldOfViewRadians, float aspectRatio, float nearDistance, float farDistance, glm::vec3 camPos, glm::vec3 camForward, glm::vec3 camUp);

        void move(float x, float y, float z);
        void rotate(float radiansX, float radiansY, float radiansZ = 0.f);
        glm::mat4 getGlobalTransform() const;

        glm::vec3 getForward() const;
        glm::mat4 getViewTransform() const;
        glm::mat4 getProjectionTransform() const;
        std::vector<glm::vec4> getFrustomCorners() const;
        glm::vec4 getFrustumCenter() const;
        glm::mat4 getWorldTransform() const;
        glm::vec3 getPosition() const;

        glm::vec4 getLocalForward() const;
        glm::vec4 getLocalUp() const;

        void getDirectionVectors(glm::vec3& forward, glm::vec3& up, glm::vec3& right) const;

    private:
        glm::mat4 mView;
        glm::mat4 mProjection;
        glm::vec3 mPosition;
        glm::vec4 mFrustumCenter;


        static constexpr float HALF_PI  = 3.14f / 2.f;
        static constexpr float DOUBLE_PI = 3.14f * 2.f;

        float mRotationY = 0.f;
        glm::mat4 mRotation;
};


#endif // CAMERA_HPP
