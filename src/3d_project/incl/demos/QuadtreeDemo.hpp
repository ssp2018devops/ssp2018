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

#ifndef QUADTREE_DEMO_HPP
#define QUADTREE_DEMO_HPP

////////////////////////////////////////////////
// Internal headers
#include "Demo.hpp"
#include "ShaderProgram.hpp"
#include "Mesh.hpp"
#include "Quadtree.hpp"
//class Camera;
#include "Camera.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <list>
#include <memory>
////////////////////////////////////////////////

struct Frustum
{
    glm::vec4 topLeft;
    glm::vec4 topRight;
    glm::vec4 botLeft;
    glm::vec4 botRight;

    glm::vec4 eye;

    Frustum transform(const glm::mat4& matrix) const
    {
        Frustum f;
        f.topLeft = matrix * topLeft;
        f.topRight = matrix * topRight;
        f.botLeft = matrix * botLeft;
        f.botRight = matrix * botRight;

        f.eye = matrix * eye;

        return f;
    }
};


class QuadtreeDemo : public Demo
{
    public:
        QuadtreeDemo(Camera& view);

        void handleInput(const SDL_Event& event);
        void update();
        void drawGeometry(glm::mat4 transform) const;

    private:
        void loadMeshes(size_t numMeshes, std::string objName);

    private:
        struct BoundingSphere
        {
            void* object;
            glm::vec4 center;
            float radius;
        };

        ShaderProgram mMeshProgram;
        ShaderProgram mLineStripProgram;
        Quadtree mQuadtree;
        Camera& mView;
        Camera mFrustum;
        Frustum mFrustumData;

        const size_t M_NUM_MESHES;
        std::list<std::unique_ptr<Mesh>> mMeshes;

        GLuint mFrustumVao;
        GLuint mFrustumVbo;
};

#endif // QUADTREE_DEMO_HPP
