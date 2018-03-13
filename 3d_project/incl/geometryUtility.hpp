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

#ifndef GEOMETRY_UTILITY_HPP
#define GEOMETRY_UTILITY_HPP

////////////////////////////////////////////////
// Internal headers
#include "Vertex.hpp"
////////////////////////////////////////////////


////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "glm/vec3.hpp"
////////////////////////////////////////////////

// Position3dType must be a type that holds three members that are number types.
// The members' names must be x, y, and z.
template<typename Position3dType>
glm::vec3 findBoundingSphereCenter(Position3dType* vertices, size_t numVertices);

template<typename Position3dType>
float findBoundingSphereRadius(Position3dType* vertices, size_t numVertices, glm::vec3 boundingSphereCenter);




#include "geometryUtility.inl"

#endif // GEOMETRY_UTILITY_HPP
