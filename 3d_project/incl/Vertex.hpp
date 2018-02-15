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

#ifndef VERTEX_HPP
#define VERTEX_HPP

struct Vertex
{
	float x, y, z; ///< Space coordinates
	float u, v; ///< Texture coordinates
	float nx, ny, nz; ///< Normal vector
};


#endif // VERTEX_HPP
