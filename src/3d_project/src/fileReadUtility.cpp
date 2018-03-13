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
#include "fileReadUtility.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "SDL2/SDL_image.h"
////////////////////////////////////////////////


ObjFileData readObj(std::string filePath)
{
    std::ifstream file(filePath);

    std::vector<float> tmpCoords;
    std::vector<float> tmpTexCoords;
    std::vector<float> tmpNormals;
    std::vector<unsigned int> vertexIndices;
    if(file)
    {
        size_t numLines = 0;
        std::ifstream lineCounter(filePath);
        std::string garbage;
        while(std::getline(lineCounter, garbage))
            numLines++;

        lineCounter.close();

        tmpCoords.reserve(numLines);
        tmpTexCoords.reserve(numLines);
        tmpNormals.reserve(numLines);
        vertexIndices.reserve(numLines);
    }
    else
        throw std::runtime_error("Could not read obj file at \"" + filePath + "\"");


    ObjFileData data;
    do
    {
        std::string line;
        std::getline(file, line);

        if(line.size() > 0)
        {
            std::istringstream strStream(line);
            std::string str;
            strStream >> str;

            if(str.size() > 0)
            {
                switch(str[0])
                {
                    case 'm':
                        if(str == "mtllib")
                        {
                            std::string mtlFile;
                            strStream >> mtlFile;

                            std::ifstream mtl("assets/meshes/" + mtlFile);

                            if(file)
                            {
                                while(std::getline(mtl, line))
                                {
                                    strStream.str(line);
                                    strStream.seekg(0, strStream.beg);
                                    strStream >> str;
                                    if(str == "map_Kd")
                                    {
                                        strStream >> data.textureFilePath;
//                                        std::cout << data.textureFilePath << std::endl;
                                    }
                                    else if(str == "Ka")
                                    {
                                        strStream >> data.kA[0] >> data.kA[1] >> data.kA[2];
                                    }
                                    else if(str == "Kd")
                                    {
                                        strStream >> data.kD[0] >> data.kD[1] >> data.kD[2];
                                    }
                                    else if(str == "Ks")
                                    {
                                        strStream >> data.kS[0] >> data.kS[1] >> data.kS[2];
                                    }
                                }
                            }


                            mtl.close();
                        }
                        break;

                    case 'v':
                        if(str.size() == 2)
                        {

                            switch(str[1])
                            {
                                case 't':
                                {
                                    float u;
                                    float v;

                                    strStream >> u >> v;
                                    v = 1 - v; // flip texture vertically due to OpenGL's coordinate system
                                    tmpTexCoords.push_back(u);
                                    tmpTexCoords.push_back(v);

                                    break;
                                }

                                case 'n':
                                {
                                    float x;
                                    float y;
                                    float z;

                                    strStream >> x >> y >> z;
                                    tmpNormals.push_back(x);
                                    tmpNormals.push_back(y);
                                    tmpNormals.push_back(z);

                                    break;
                                }

                            }
                        }
                        else
                        {
                            float x;
                            float y;
                            float z;

                            strStream >> x >> y >> z;
                            tmpCoords.push_back(x);
                            tmpCoords.push_back(y);
                            tmpCoords.push_back(z);
                        }
                        break;

                    case 'g':

                        break;

                    case 'u':
                        if(str == "usemtl")
                        {

                        }
                        break;

                    case 'f':
                        for(int i = 0; i < 3; i++)
                        {
                            char separator;
                            unsigned int vertexIndex;
                            strStream >> vertexIndex >> separator;
                            vertexIndices.push_back((vertexIndex - 1) * 3); // coordinate

                            strStream >> vertexIndex >> separator;
                            vertexIndices.push_back((vertexIndex - 1) * 2); // texture coordinate

                            strStream >> vertexIndex;
                            vertexIndices.push_back((vertexIndex - 1) * 3); // normal
                        }



                        {
                            char separator;
                            strStream >> separator;

                            if(strStream)
                            {
                                strStream.putback(separator);

                                size_t vertexIndicesSize = vertexIndices.size();
                                for(int i = vertexIndicesSize - 9; i < vertexIndicesSize - 6; i++)
                                    vertexIndices.push_back(vertexIndices[i]);

                                for(int i = vertexIndicesSize - 3; i < vertexIndicesSize; i++)
                                    vertexIndices.push_back(vertexIndices[i]);

                                char separator;
                                unsigned int vertexIndex;

                                strStream >> vertexIndex >> separator;
                                vertexIndices.push_back((vertexIndex - 1) * 3); // coordinate

//                                std::cout << " " << vertexIndex;

                                strStream >> vertexIndex >> separator;
                                vertexIndices.push_back((vertexIndex - 1) * 2); // texture coordinate

//                                std::cout << " " << vertexIndex;

                                strStream >> vertexIndex;
                                vertexIndices.push_back((vertexIndex - 1) * 3); // normal

//                                std::cout << " " << vertexIndex << std::endl;
                            }
                        }

                        {
                            char separator;
                            strStream >> separator;
                        }

                        while(strStream)
                        {
//                            std::cout << ":";
                            char separator;
                            unsigned int vertexIndex;
                            strStream >> vertexIndex >> separator;
//                            vertexIndices.push_back((vertexIndex - 1) * 3); // coordinate
//                            std::cout << " " << vertexIndex;
                            strStream >> vertexIndex >> separator;
//                            vertexIndices.push_back((vertexIndex - 1) * 2); // texture coordinate
//                            std::cout << " " << vertexIndex;
                            strStream >> vertexIndex;
//                            vertexIndices.push_back((vertexIndex - 1) * 3); // normal
//                            std::cout << " " << vertexIndex << std::endl;
                        }

                        break;
                    default:
                        break;
                }
            }


//            std::cout << str << std::endl;
        }
    } while(file);

    file.close();

//    std::cout << "COORDS: " << std::endl;
//    for(int i = 0; i < tmpCoords.size(); i += 3)
//    {
//        std::cout << "Coord " << i << ": " << tmpCoords[i] << " " << tmpCoords[i + 1] << " " << tmpCoords[i + 2] << std::endl;
//    }
//
//    std::cout << "TEX COORDS: " << std::endl;
//    for(int i = 0; i < tmpTexCoords.size(); i += 2)
//    {
//        std::cout << "Tex coord " << i << ": " << tmpTexCoords[i] << " " << tmpTexCoords[i + 1] << std::endl;
//    }
//
//    std::cout << "NORMALS: " << std::endl;
//    for(int i = 0; i < tmpNormals.size(); i += 3)
//    {
//        std::cout << "Normal " << i << ": " << tmpNormals[i] << " " << tmpNormals[i + 1] << " " << tmpNormals[i + 2] << std::endl;
//    }

//    std::cout   << "Num vertices: " << vertexIndices.size() / 3 << std::endl
//                << "Num faces: " << vertexIndices.size() / 9 << std::endl;



    data.vertices.resize(vertexIndices.size() / 3);

    for(int i = 0, j = 0; i < data.vertices.size() && j < vertexIndices.size() - 2; i++, j += 3)
    {
        Vertex& v = data.vertices[i];
        unsigned int iCoords = vertexIndices[j];
        v.x = tmpCoords[iCoords];
        v.y = tmpCoords[iCoords + 1];
        v.z = tmpCoords[iCoords + 2];

        unsigned int iTexCoords = vertexIndices[j + 1];
        v.u = tmpTexCoords[iTexCoords];
        v.v = tmpTexCoords[iTexCoords + 1];

        unsigned int iNormals = vertexIndices[j + 2];
        v.nx = tmpNormals[iNormals];
        v.ny = tmpNormals[iNormals + 1];
        v.nz = tmpNormals[iNormals + 2];

    }
//    std::cout << "Num vertices: " << vertices.size() << std::endl;
    for(int i = 0; i < data.vertices.size(); i++)
    {
        Vertex v = data.vertices[i];
//        std::cout << "Vertex " << i << ": " << std::endl;
//        std::cout << "\tCoord: " << v.x << " " << v.y << " " << v.z << std::endl;
//        std::cout << "\tTex: " << v.u << " " << v.v << std::endl;
//        std::cout << "\tNormal: " << v.nx << " " << v.ny << " " << v.nz << std::endl;
    }

    return data;
}
