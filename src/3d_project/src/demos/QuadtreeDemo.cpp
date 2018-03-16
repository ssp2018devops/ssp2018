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
#include "demos/QuadtreeDemo.hpp"
#include "fileReadUtility.hpp"
#include "geometryUtility.hpp"
#include "Camera.hpp"
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

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtx/rotate_vector.hpp"



float kAss[3];
float kDss[3];
float kSss[3];

size_t frustumCornersSize = 0;
QuadtreeDemo::QuadtreeDemo(Camera& view)
: mMeshProgram(ShaderProgram::Id::DEFERRED_RENDER)
, mLineStripProgram(ShaderProgram::Id::LINESTRIP, true, false, true)
, mQuadtree(glm::vec3(-1000.f, 0.f, -1000.f), 2000.f, 2000.f)
, mView(view)
, M_NUM_MESHES(1000)
, mFrustum(glm::pi<float>() * 0.45f,  ((float)640) / ((float)640), 0.5f, 200.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f))
{
    mFrustum = mView;

    const std::string texturesDir = "assets/textures/";

    mMeshProgram.use();
    loadMeshes(M_NUM_MESHES, "box");

    float meshDisplacementX = 0.f;
    for(auto& mesh : mMeshes)
    {
        mesh->move(glm::vec3(meshDisplacementX, 0.f, 0.f));
        mQuadtree.insert(mesh.get(), mesh->getBoundingSphereCenter(), mesh->getBoundingSphereRadius());

        meshDisplacementX += 1.3f;
    }

    std::vector<glm::vec4> frustumCorners = mView.getFrustomCorners();

    glGenVertexArrays(1, &mFrustumVao);
    glBindVertexArray(mFrustumVao);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &mFrustumVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mFrustumVbo);

    frustumCornersSize = frustumCorners.size();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * frustumCorners.size(), frustumCorners.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(mLineStripProgram.getId(), "vertex_position"), 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (char*)nullptr);


    glm::mat4 projectionInverse = glm::inverse(mView.getProjectionTransform());

    glm::vec4 p(-1.f, 1.f, 1.f, 1.f);
    p = projectionInverse * p;
    p /= p.w;
    p.w = 0.f;
    mFrustumData.topLeft = p;

    p = glm::vec4(1.f, 1.f, 1.f, 1.f);
    p = projectionInverse * p;
    p /= p.w;
    p.w = 0.f;
    mFrustumData.topRight = p;


    p = glm::vec4(1.f, -1.f, 1.f, 1.f);
    p = projectionInverse * p;
    p /= p.w;
    p.w = 0.f;
    mFrustumData.botRight = p;

    p = glm::vec4(-1.f, -1.f, 1.f, 1.f);
    p = projectionInverse * p;
    p /= p.w;
    p.w = 0.f;
    mFrustumData.botLeft = p;

    mFrustumData.eye = glm::vec4(0.f, 0.f, 0.f, 1.f);
}


void QuadtreeDemo::handleInput(const SDL_Event& event)
{
    const uint8_t* isKeyDown = SDL_GetKeyboardState(nullptr);

    float speed = 5.f;

    if(isKeyDown[SDL_SCANCODE_LEFT])
        mFrustum.move(-speed, 0.f, 0.f);
    else if(isKeyDown[SDL_SCANCODE_RIGHT])
        mFrustum.move(speed, 0.f, 0.f);

    if(isKeyDown[SDL_SCANCODE_UP])
        mFrustum.move(0.f, 0.f, -speed);
    else if(isKeyDown[SDL_SCANCODE_DOWN])
        mFrustum.move(0.f, 0.f, speed);

    float rotationSpeed = glm::pi<float>() / 100.f;
    if(isKeyDown[SDL_SCANCODE_KP_4])
        mFrustum.rotate(-rotationSpeed, 0, 0);
    else if(isKeyDown[SDL_SCANCODE_KP_6])
        mFrustum.rotate(rotationSpeed, 0, 0);
    else if(isKeyDown[SDL_SCANCODE_KP_8])
        mFrustum.rotate(0, rotationSpeed, 0);
    else if(isKeyDown[SDL_SCANCODE_KP_2])
        mFrustum.rotate(0, -rotationSpeed, 0);

    if(isKeyDown[SDL_SCANCODE_SPACE])
        mFrustum.move(0, speed, 0);
    else if(isKeyDown[SDL_SCANCODE_LSHIFT])
        mFrustum.move(0, -speed, 0);
}


void QuadtreeDemo::update()
{
    mQuadtree.clear();
    for(auto& mesh : mMeshes)
    {
        mesh->rotate(0.003f, glm::vec3(0.f, 1.f, 0.f));
        mQuadtree.insert(mesh.get(), mesh->getBoundingSphereCenter(), mesh->getBoundingSphereRadius());
    }

}

bool isPointInRect(glm::vec3 p, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botLeft)
{
    if(glm::dot(topLeft - botLeft, topLeft - botLeft) == 0.f)
        return false;

    if(glm::dot(topLeft - topRight, topLeft - topRight) == 0.f)
        return false;

    if(glm::dot(p - topLeft, topRight - topLeft) < 0.f)
        return false;

    if(glm::dot(p - topRight, topLeft - topRight) < 0.f)
        return false;

    if(glm::dot(p - topLeft, botLeft - topLeft) < 0.f)
        return false;

    if(glm::dot(p - botLeft, topLeft - botLeft) < 0.f)
        return false;

    return true;
}

void QuadtreeDemo::drawGeometry(glm::mat4 transform) const
{
    mLineStripProgram.use();

    glm::mat4 frustumTransform = mView.getWorldTransform();

    Frustum frustum = mFrustumData.transform(frustumTransform);
    frustum.botLeft.y = frustum.botRight.y = frustum.topLeft.y = frustum.topRight.y = frustum.eye.y = 0.f;

    std::list<Quadtree::Quad> quads;
    std::set<void*> nearbyObjects;
    if(isPointInRect(glm::vec3(frustum.eye), glm::vec3(frustum.topLeft), glm::vec3(frustum.topRight), glm::vec3(frustum.botLeft)))
    {
        glm::vec3 topLeft(frustum.eye + frustum.topLeft);
        glm::vec3 right(frustum.topRight - frustum.topLeft);
        glm::vec3 down(frustum.botLeft - frustum.topLeft);

        quads = mQuadtree.getIntersectingQuads(topLeft, right, down);
        nearbyObjects = mQuadtree.getNearbyObjects(topLeft, right, down);
    }
    else
    {
        enum SideIndex
        {
            TOP,
            RIGHT,
            DOWN,
            LEFT,
            NUM_SIDES,
        };

        float tMap[NUM_SIDES] =
        {
            glm::dot(frustum.topLeft, frustum.topRight),
            glm::dot(frustum.topRight, frustum.botRight),
            glm::dot(frustum.botRight, frustum.botLeft),
            glm::dot(frustum.botLeft, frustum.topLeft)
        };

        float tMin = tMap[0];
        size_t widestSide = TOP;
        for(size_t i = TOP; i < NUM_SIDES; i++)
        {
            if(tMap[i] < tMin)
            {
                tMin = tMap[i];
                widestSide = i;
            }
        }


        switch(widestSide)
        {
            case TOP:
                quads = mQuadtree.getIntersectingQuads
                (
                    glm::vec3(frustum.eye),
                    glm::vec3(frustum.topLeft),
                    glm::vec3(frustum.topRight),
                    glm::dot(frustum.botLeft, frustum.botLeft)
                );
                nearbyObjects = mQuadtree.getNearbyObjects
                (
                    glm::vec3(frustum.eye),
                    glm::vec3(frustum.topLeft),
                    glm::vec3(frustum.topRight),
                    glm::dot(frustum.botLeft, frustum.botLeft)
                );
                break;

            case RIGHT:
                std::cout << "RIGHT SIDE IS WIDEST?? Should only be possible if frustum is rotated by Z. Which it can't be atm." << std::endl;
                break;

            case DOWN:
                quads = mQuadtree.getIntersectingQuads
                (
                    glm::vec3(frustum.eye),
                    glm::vec3(frustum.botLeft),
                    glm::vec3(frustum.botRight),
                    glm::dot(frustum.topLeft, frustum.topLeft)
                );
                nearbyObjects = mQuadtree.getNearbyObjects
                (
                    glm::vec3(frustum.eye),
                    glm::vec3(frustum.botLeft),
                    glm::vec3(frustum.botRight),
                    glm::dot(frustum.topLeft, frustum.topLeft)
                );
                break;

            case LEFT:
                std::cout << "LEFT SIDE IS WIDEST?? Should only be possible if frustum is rotated by Z. Which it can't be atm." << std::endl;
                break;

            default:
                break;
        }
    }

    glm::mat4 mvp = mView.getGlobalTransform();
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::getCurrentProgramId(), "MVP"), 1, GL_FALSE, &mvp[0][0]);
    for(Quadtree::Quad quad : quads)
    {
        glm::vec3 p1 = quad.position;

        quad.position.x += quad.width;
        glm::vec3 p2 = quad.position;

        quad.position.z += quad.height;
        glm::vec3 p3 = quad.position;

        quad.position.x -= quad.width;
        glm::vec3 p4 = quad.position;

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glm::vec3 vertices[4] = {p1, p2, p3, p4};

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(glGetAttribLocation(ShaderProgram::getCurrentProgramId(), "vertex_position"), 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (char*)nullptr);


        glDrawArrays(GL_LINE_LOOP, 0, 4);

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    glBindVertexArray(mFrustumVao);

    frustumTransform = transform * frustumTransform;
    glUniformMatrix4fv(ShaderProgram::getCurrentProgramUniformLocation("MVP"), 1, GL_FALSE, &frustumTransform[0][0]);

    glDrawArrays(GL_LINE_STRIP, 0, frustumCornersSize);



    mMeshProgram.use();
    GLint mvpHandle = ShaderProgram::getCurrentProgramUniformLocation("MVP");
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kA"), 1, kAss);
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kD"), 1, kDss);
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("kS"), 1, kSss);

    for(void* object : nearbyObjects)
        ((Mesh*)object)->draw(mvpHandle, transform);

    std::cout << "Number of boxes drawn: " << nearbyObjects.size() << std::endl;
//
//    for(auto& mesh : mMeshes)
//        mesh->draw(mvpHandle, transform);
}

void QuadtreeDemo::loadMeshes(size_t numMeshes, std::string objName)
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
        kAss[i] = k[i];

    k = meshData.kD;
    std::cout << "kD: " << k[0] << " " << k[1] << " " << k[2] << std::endl;
    for(size_t i = 0; i < 3; i++)
        kDss[i] = k[i];

    k = meshData.kS;
    std::cout << "kS: " << k[0] << " " << k[1] << " " << k[2] << std::endl;
    for(size_t i = 0; i < 3; i++)
        kSss[i] = k[i];


    GLuint meshTexture;
    glGenTextures(1, &meshTexture);

    glBindTexture(GL_TEXTURE_2D, meshTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



    const std::string texturesDir = "assets/textures/";
    SDL_Surface* image = IMG_Load((texturesDir + meshData.textureFilePath).c_str());
    if(!image)
    {
        printf("IMG_Load: %s\n", IMG_GetError());

        image = IMG_Load((texturesDir + meshName + ".tga").c_str());
        if(!image)
        {
            printf("IMG_Load: %s\n", IMG_GetError());
//            glDeleteTextures(1, &meshTexture);

            unsigned char pixel[4];
            for(size_t i = 0; i < 3; i ++)
                pixel[i] = 255.f * meshData.kD[i];

            pixel[4] = 255;

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);
        }
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    }
    SDL_FreeSurface(image);

    GLuint normalMapId = 0;
    glGenTextures(1, &normalMapId);
    glBindTexture(GL_TEXTURE_2D, normalMapId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SDL_Surface* normal = IMG_Load((texturesDir + objName + "_normal.png").c_str());
    if(!normal)
    {
        printf("IMG_Load: %s\n", IMG_GetError());
        unsigned char pixel[3] = {128, 128, 255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    }
    else
    {
        GLenum format;

        switch(normal->format->BytesPerPixel)
        {
            case 3:
                format = GL_RGB;
                break;

            case 4:
                format = GL_RGBA;
                break;

            default:
                throw std::runtime_error("Unsupported pixel format.");
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, normal->w, normal->h, 0, format,  GL_UNSIGNED_BYTE, normal->pixels);
    }
    SDL_FreeSurface(normal);


    mMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(vertices.data(), vertices.size(), meshTexture, normalMapId)));
    Mesh* firstMesh = mMeshes.back().get();
    for(size_t i = 1; i < numMeshes; i++)
    {
        mMeshes.push_back(std::unique_ptr<Mesh>(new Mesh(*firstMesh)));
    }


}

