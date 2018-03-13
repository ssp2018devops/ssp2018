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
// Simple DirectMedia Layer (SDL)
#include "glm/geometric.hpp"
////////////////////////////////////////////////

template<typename Position3dType>
glm::vec3 findBoundingSphereCenter(Position3dType* vertices, size_t numVertices)
{
    glm::vec3 sum;
    for(size_t i = 0; i < numVertices; i++)
    {
        sum.x += vertices[i].x;
        sum.y += vertices[i].y;
        sum.z += vertices[i].z;
    }

    return sum / (float)numVertices;
}

template<typename Position3dType>
float findBoundingSphereRadius(Position3dType* vertices, size_t numVertices, glm::vec3 boundingSphereCenter)
{
    float maxDistanceSqrd = 0.f;
    for(size_t i = 0; i < numVertices; i++)
    {
        glm::vec3 centerToVertex = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z) - boundingSphereCenter;

        float distanceSqrd = glm::dot(centerToVertex, centerToVertex);
        if(distanceSqrd > maxDistanceSqrd)
            maxDistanceSqrd = distanceSqrd;
    }

    return sqrtf(maxDistanceSqrd);
}

