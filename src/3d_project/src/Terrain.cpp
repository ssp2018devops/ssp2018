
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
#include "Terrain.hpp"
#include "ShaderProgram.hpp"
#include "imageUtility.hpp"
////////////////////////////////////////////////


#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <cassert>

Terrain::Terrain(std::string heightmapPath, std::string texturePath)
{
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	loadHeightmap(heightmapPath);
	loadTexture(texturePath);

	mImageWidthHandle = ShaderProgram::getCurrentProgramUniformLocation("image_width");

    //glUniform1i(glGetUniformLocation(shaderProgram, "normalSampler"), 1);

    move(glm::vec3(-(float)mImageWidth / 2.f, -255.f / 2.f, -(float)mImageWidth / 2.f));
}

void Terrain::loadHeightmap(std::string heightmapPath)
{
    Image heightmap = readImage(heightmapPath);

    if(heightmap.data->h < 2)
        throw std::runtime_error("Heightmap height must be at least 2, or the formula \"numVertices(width, height) = height * width + (height - 2) * (2 + width)\" won't work. :-)");

    if(heightmap.data->w < 2)
        throw std::runtime_error("Heightmap width must be at least 2, man.");

    mNumVertices = (heightmap.data->h * heightmap.data->w + (heightmap.data->h - 2) * (2 + heightmap.data->w));
    mImageWidth = heightmap.data->w;
    mImageSize = heightmap.data->w * heightmap.data->h;

    mHeightmap = getPixels(heightmap);
    unsigned char* vertices = generateMesh(mHeightmap, heightmap.data->h);


	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * mNumVertices, vertices, GL_STATIC_DRAW);
    delete[] vertices;

	GLint vertexHeight = ShaderProgram::getCurrentProgramAttribLocation("vertex_height");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(vertexHeight, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(unsigned char), 0);
}


void Terrain::loadTexture(std::string texturePath)
{
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    Image texture = readImage(texturePath);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.data->w, texture.data->h, 0, GL_RGB,  GL_UNSIGNED_BYTE, texture.data->pixels);
}


unsigned char* Terrain::generateMesh(const unsigned char* heightmapPixels, size_t heightmapHeight) const
{
    unsigned char* vertices = new unsigned char[mNumVertices];
    for(size_t y = 0, i = 0; y < heightmapHeight - 1; y++)
    {
        for(size_t x = 0; x < mImageWidth; x++)
        {
            vertices[i] = heightmapPixels[y * mImageWidth + x];
            i++;
            vertices[i] = heightmapPixels[(y + 1) * mImageWidth + x];
            i++;

        }
        if(i < mNumVertices - 1)
        {
            vertices[i] = vertices[i - 1];
            i++;
            vertices[i] = heightmapPixels[(y + 1) * mImageWidth];
            i++;
        }
    }

    return vertices;
}



Terrain::~Terrain()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);

    delete[] mHeightmap;
}

void Terrain::draw(GLuint mvpHandle, glm::mat4 transform) const
{
	glBindVertexArray(mVao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glm::mat4 worldTransform = getWorldTransform();
//    worldTransform = glm::translate(worldTransform, glm::vec3(mImageWidth / 2, 0, -(float)mImageWidth / 2));

	transform = transform * worldTransform;
	glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &transform[0][0]);


    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::getCurrentProgramId(), "modelTransform"), 1, GL_FALSE, &worldTransform[0][0]);

	glUniform1i(mImageWidthHandle, mImageWidth);

	// draw 3 vertices starting from index 0 in the vertex array currently bound (VAO), with current in-use shader
	glDrawArrays(GL_TRIANGLE_STRIP, 0, mNumVertices);
}

float Terrain::getHeight(float x, float z) const
{
    glm::mat4 worldTransformInverse = glm::inverse(getWorldTransform());

    glm::vec3 localPosition = glm::vec3(worldTransformInverse * glm::vec4(x, 0, z, 1.f));

    if(localPosition.x < 0.f || localPosition.z < 0.f)
    {
        std::cout << "Cannot get terrain height. Player is out of bounds." << std::endl;
        return 0.f;
    }

    size_t indexX = (size_t)(localPosition.x);
    size_t indexZ = (size_t)(localPosition.z);

    if(indexZ * mImageWidth + indexX >= mImageSize)
    {
        std::cout << "Cannot get terrain height. Player is out of bounds." << std::endl;
        return 0.f;
    }

    glm::vec3 topLeftPoint = glm::vec3(indexX, mHeightmap[indexZ * mImageWidth + indexX], indexZ);
    glm::vec3 quadCoord = localPosition - topLeftPoint;
    glm::vec4 projectionPoint = glm::vec4(localPosition, 1.f);
    float t = quadCoord.x + quadCoord.z - 1.f;
    if(t < 0.f)
    {
        float topRightHeight = mHeightmap[indexZ * mImageWidth + indexX + 1];
        float bottomLeftHeight = mHeightmap[(indexZ + 1) * mImageWidth + indexX];

        projectionPoint.y = topLeftPoint.y;
        projectionPoint.y += (topRightHeight - topLeftPoint.y) * quadCoord.x;
        projectionPoint.y += (bottomLeftHeight - topLeftPoint.y) * quadCoord.z;
    }
    else if(t > 0.f)
    {
        float topRightHeight = mHeightmap[indexZ * mImageWidth + indexX + 1];
        float bottomLeftHeight = mHeightmap[(indexZ + 1) * mImageWidth + indexX];
        float bottomRightHeight = mHeightmap[(indexZ + 1) * mImageWidth + indexX + 1];

        projectionPoint.y = bottomRightHeight;
        projectionPoint.y += (bottomLeftHeight - bottomRightHeight) * (1.f - quadCoord.x);
        projectionPoint.y += (topRightHeight - bottomRightHeight) * (1.f - quadCoord.z);
    }
    else
    {
        float heightX = mHeightmap[indexZ * mImageWidth + indexX + 1];
        projectionPoint.y = topLeftPoint.y;
        projectionPoint.y += (heightX - topLeftPoint.y) * quadCoord.x;
    }

    projectionPoint = getWorldTransform() * projectionPoint;
    return projectionPoint.y;
}
