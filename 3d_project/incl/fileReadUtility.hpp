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

#ifndef FILE_READ_UTILITY_HPP
#define FILE_READ_UTILITY_HPP

////////////////////////////////////////////////
// Internal headers
#include "Vertex.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <string>
#include <vector>
////////////////////////////////////////////////

struct ObjFileData
{
    std::string textureFilePath;
    std::vector<Vertex> vertices;
    float kA[3] = {0.1f, 0.1f, 0.1f};
    float kD[3] = {0.f, 0.f, 0.f};
    float kS[3] = {0.f, 0.f, 0.f};
};

ObjFileData readObj(std::string filePath);


#endif // FILE_READ_UTILITY_HPP
