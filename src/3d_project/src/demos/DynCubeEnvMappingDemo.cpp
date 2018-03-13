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
#include "demos/DynCubeEnvMappingDemo.hpp"
#include "fileReadUtility.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "ReflectionMesh.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "SDL2/SDL_image.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL
#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <iostream>
////////////////////////////////////////////////

#include "glm/gtc/constants.hpp"
#include "Framebuffer.hpp"

float kA[3];
float kD[3];
float kS[3];

GLuint cubeMapId = 0;
ReflectionMesh* reflectionMesh = nullptr;

DynCubeEnvMappingDemo::DynCubeEnvMappingDemo(const Framebuffer& deferredBuffer, const ShaderProgram& lightPassProgram)
: mMeshProgram(ShaderProgram::Id::DEFERRED_RENDER)
, mCubeMapProgram(ShaderProgram::Id::CUBE_MAP)
, mLightPassProgram(lightPassProgram)
, M_NUM_MESHES(1000)
, mDeferredBuffer(deferredBuffer)
, mCubeMapBuffer(640, 640, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 1)
{
    mMeshProgram.use();
    loadMeshes(M_NUM_MESHES, "box");

    float meshDisplacementX = 0.f;
    for(auto& mesh : mMeshes)
    {
        mesh->move(glm::vec3(meshDisplacementX, 2.f, 0.f));
        meshDisplacementX += 1.3f;

    }

//    auto it = mMeshes.begin();
//    it++;
//
//    (*it)->move(glm::vec3(-1.3f, -4.f, 0.f));
//    it++;
//    (*it)->move(glm::vec3(-1.3f, -2.f, 0.f));
//    it++;
//    (*it)->move(glm::vec3(-1.3f * 4, -2.f, 0.f));
//    it++;
//    (*it)->move(glm::vec3(-1.3f * 4, -2.f, 2.f));
//    it++;
//    (*it)->move(glm::vec3(-1.3f * 5, -2.f, -2.f));


    ObjFileData meshData = readObj("assets/meshes/box.obj");
    std::vector<Vertex>& vertices = meshData.vertices;

    SDL_Surface* normal = IMG_Load("assets/textures/box_normal.png");
    if(!normal)
    {
        printf("IMG_Load: %s\n", IMG_GetError());
        throw std::runtime_error("Failed to load BOX NORMAL MAP.");
    }

    GLuint normalMapId = 0;
    glGenTextures(1, &normalMapId);

    glBindTexture(GL_TEXTURE_2D, normalMapId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, normal->w, normal->h, 0, GL_RGBA,  GL_UNSIGNED_BYTE, normal->pixels);
    SDL_FreeSurface(normal);

    glGenTextures(1, &cubeMapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    const size_t SIZE = 640;
    for(unsigned int i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, SIZE, SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    reflectionMesh = new ReflectionMesh(vertices.data(), vertices.size(), cubeMapId, normalMapId);
    reflectionMesh->move(glm::vec3(1.f, 4.f, 0.f));
}


void DynCubeEnvMappingDemo::handleInput(const SDL_Event& event)
{

}


void DynCubeEnvMappingDemo::update()
{
    for(auto& mesh : mMeshes)
    {
        mesh->rotate(0.003f, glm::vec3(0.f, 1.f, 0.f));
    }

    reflectionMesh->rotate(0.003f, glm::vec3(0.f, 1.f, 0.f));
}

void DynCubeEnvMappingDemo::draw(glm::mat4 transform) const
{
    mMeshProgram.use();
    GLint mvpHandle = ShaderProgram::getCurrentProgramUniformLocation("MVP");
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kA"), 1, kA);
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kD"), 1, kD);
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kS"), 1, kS);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mDeferredBuffer.getFbo());
    {
        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        for(auto& mesh : mMeshes)
            mesh->draw(mvpHandle, transform);


        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
    }
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    for(unsigned int i = 0; i < mDeferredBuffer.getNumTextures(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mDeferredBuffer.getAttachmentImageId(i));
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mCubeMapBuffer.getFbo());

    glBindFramebuffer(GL_READ_FRAMEBUFFER, mDeferredBuffer.getFbo());
    {
        mLightPassProgram.use();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void DynCubeEnvMappingDemo::drawGeometry(glm::mat4 transform) const
{
    GLint previousReadFbo;
    GLint previousDrawFbo;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previousReadFbo);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousDrawFbo);

    for(size_t i = 0; i < 6; i++)
    {
        mCubeMapBuffer.setAttachmentTexture(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMapId, 0);
        draw(reflectionMesh->getCameraTransform(i));
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, previousReadFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, previousDrawFbo);

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    mCubeMapProgram.use();
    GLint mvpHandle = ShaderProgram::getCurrentProgramUniformLocation("MVP");
    reflectionMesh->draw(mvpHandle, transform);

    mMeshProgram.use();
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kA"), 1, kA);
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kD"), 1, kD);
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kS"), 1, kS);

    mvpHandle = ShaderProgram::getCurrentProgramUniformLocation("MVP");
    for(auto& mesh : mMeshes)
        mesh->draw(mvpHandle, transform);

}

void DynCubeEnvMappingDemo::loadMeshes(size_t numMeshes, std::string objName)
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
        kA[i] = k[i];

    k = meshData.kD;
    std::cout << "kD: " << k[0] << " " << k[1] << " " << k[2] << std::endl;
    for(size_t i = 0; i < 3; i++)
        kD[i] = k[i];

    k = meshData.kS;
    std::cout << "kS: " << k[0] << " " << k[1] << " " << k[2] << std::endl;
    for(size_t i = 0; i < 3; i++)
        kS[i] = k[i];

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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, normal->w, normal->h, 0, GL_RGBA,  GL_UNSIGNED_BYTE, normal->pixels); // png
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, normal->w, normal->h, 0, GL_RGB,  GL_UNSIGNED_BYTE, normal->pixels); //jpeg

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

