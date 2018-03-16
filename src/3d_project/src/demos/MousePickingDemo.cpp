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
#include "demos/MousePickingDemo.hpp"
#include "Camera.hpp"
#include "imageUtility.hpp"
#include "Vertex.hpp"
#include "fileReadUtility.hpp"
#include "Framebuffer.hpp"
////////////////////////////////////////////////



////////////////////////////////////////////////
// OpenGL
#include "GL/glew.h"
//#define GL_GLEXT_PROTOTYPES
//#include "GL/gl.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "SDL_image.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <iostream>
////////////////////////////////////////////////


float kAs[3];
float kDs[3];
float kSs[3];

MousePickingDemo::MousePickingDemo(const Framebuffer& frameBuffer)
: mFramebuffer(frameBuffer)
, mMeshProgram(ShaderProgram::Id::DEFERRED_RENDER)
, M_NUM_MESHES(1000)
{
    const std::string texturesDir = "assets/textures/";

    mMeshProgram.use();
    loadMeshes(M_NUM_MESHES, "box");

    float meshDisplacementX = 0.f;
    for(auto& mesh : mMeshes)
    {
        mesh->move(glm::vec3(meshDisplacementX, 0.f, 0.f));
        meshDisplacementX += 1.3f;
    }
}


void MousePickingDemo::handleInput(const SDL_Event& event)
{

}


void MousePickingDemo::update()
{
}


void MousePickingDemo::drawGeometry(glm::mat4 transform) const
{
    mMeshProgram.use();
    GLint mvpHandle = ShaderProgram::getCurrentProgramUniformLocation("MVP");
    GLuint objectId = 1;

    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kA"), 1, kAs);
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kD"), 1, kDs);
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kS"), 1, kSs);
    for(auto& mesh : mMeshes)
    {
        glUniform1ui(ShaderProgram::getCurrentProgramUniformLocation("id"), objectId);

        mesh->draw(mvpHandle, transform);
        objectId++;
    }

    float id;
    mFramebuffer.getPixels(4, mFramebuffer.getWidth() / 2, mFramebuffer.getHeight() / 2, 1, 1, &id);
    std::cout << (unsigned int)id << std::endl;

}

void MousePickingDemo::loadMeshes(size_t numMeshes, std::string objName)
{
    if(numMeshes < 1)
        return;

    const std::string meshesDir = "assets/meshes/";
    const std::string meshName = objName;
    ObjFileData meshData = readObj(meshesDir + meshName + ".obj");
    std::vector<Vertex>& vertices = meshData.vertices;

    float* k;


    k = meshData.kA;
    std::cout << "kA: " << k[0] << " " << k[1] << " " << k[2] << std::endl;
    for(size_t i = 0; i < 3; i++)
        kAs[i] = k[i];

    k = meshData.kD;
    std::cout << "kD: " << k[0] << " " << k[1] << " " << k[2] << std::endl;
    for(size_t i = 0; i < 3; i++)
        kDs[i] = k[i];

    k = meshData.kS;
    std::cout << "kS: " << k[0] << " " << k[1] << " " << k[2] << std::endl;
    for(size_t i = 0; i < 3; i++)
        kSs[i] = k[i];

    GLuint meshTexture;
    glGenTextures(1, &meshTexture);

    glBindTexture(GL_TEXTURE_2D, meshTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLuint normalMapId = 0;

    const std::string texturesDir = "assets/textures/";
    SDL_Surface* image = IMG_Load((texturesDir + meshData.textureFilePath).c_str());
    if(!image)
    {
        printf("IMG_Load: %s\n", IMG_GetError());

        image = IMG_Load((texturesDir + meshName + ".tga").c_str());
        if(!image)
        {
            printf("IMG_Load: %s\n", IMG_GetError());
            glDeleteTextures(1, &meshTexture);
            return;
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);
        }
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);


        SDL_Surface* normal = IMG_Load((texturesDir + "box_normal.png").c_str());
        if(!normal)
        {
            printf("IMG_Load: %s\n", IMG_GetError());
            throw std::runtime_error("Failed to load BOX NORMAL MAP.");
        }

        glGenTextures(1, &normalMapId);

        glBindTexture(GL_TEXTURE_2D, normalMapId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, normal->w, normal->h, 0, GL_RGBA,  GL_UNSIGNED_BYTE, normal->pixels);

        SDL_FreeSurface(normal);
    }

    mMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(vertices.data(), vertices.size(), meshTexture, normalMapId)));
    Mesh* firstMesh = mMeshes.back().get();
    for(size_t i = 1; i < numMeshes; i++)
    {
        mMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(*firstMesh)));
    }

    SDL_FreeSurface(image);
}

