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
#include "CubeMapFramebuffer.hpp"
#include "ShaderProgram.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <stdexcept>
#include <iostream>
////////////////////////////////////////////////


CubeMapFramebuffer::CubeMapFramebuffer(size_t width, size_t height, GLint textureInternalFormat, GLenum texturePixelFormat, GLenum textureDataType, unsigned int numTextures)
: mImageWidth(width)
, mImageHeight(height)
, mTextureInternalFormat(textureInternalFormat)
, mTexturePixelFormat(texturePixelFormat)
, mTextureDataType(textureDataType)
{
    if(numTextures < 1)
        throw std::runtime_error("A CubeMapFramebuffer must contain at least one texture.");

    glGenFramebuffers(1, &mFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

    setNumTextures(numTextures);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "FB error, status: " << status << std::endl;
    }
}

////////////////////////////////////////////////

CubeMapFramebuffer::~CubeMapFramebuffer()
{
    glDeleteTextures(mTextures.size(), mTextures.data());
    if(mDepthTexture)
        glDeleteTextures(1, &mDepthTexture);

    if(mStencilTexture)
        glDeleteTextures(1, &mStencilTexture);

    glDeleteCubeMapFramebuffers(1, &mFbo);
}

////////////////////////////////////////////////

GLuint CubeMapFramebuffer::getFbo() const
{
    return mFbo;
}

////////////////////////////////////////////////

void CubeMapFramebuffer::setNumTextures(unsigned int numTextures)
{
    glDeleteTextures(mTextures.size(), mTextures.data());

    mTextures.resize(numTextures);
    glGenTextures(numTextures, mTextures.data());

    for(unsigned int i = 0; i < numTextures; i++)
    {

glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

        glBindTexture(GL_TEXTURE_2D, mTextures[i]);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, mTextureInternalFormat, mImageWidth, mImageHeight, 0, mTexturePixelFormat, mTextureDataType, nullptr);
        glCubeMapFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTextures[i], 0);
    }

    glGenTextures(1, &mDepthTexture);
    glBindTexture(GL_TEXTURE_2D, mDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, mImageWidth, mImageHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glCubeMapFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);


    GLenum* textureAttachmentTypes = new GLenum[numTextures];
    for(unsigned int i = 0; i < numTextures; i++)
        textureAttachmentTypes[i] = GL_COLOR_ATTACHMENT0 + i;

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);
    glDrawBuffers(numTextures, textureAttachmentTypes);

    delete[] textureAttachmentTypes;
}

////////////////////////////////////////////////

GLuint CubeMapFramebuffer::getTextureId(unsigned int i) const
{
    if(i >= mTextures.size())
        throw std::runtime_error("Index out of bounds.");

    return mTextures[i];
}

////////////////////////////////////////////////

size_t CubeMapFramebuffer::getNumTextures() const
{
    return mTextures.size();
}

////////////////////////////////////////////////

void CubeMapFramebuffer::bindTextureToShaderUniform(unsigned int textureIndex, std::string uniformName, const ShaderProgram& shader) const
{
    if(textureIndex >= mTextures.size())
        throw std::runtime_error("Index out of bounds.");

    GLint uniformLocation = glGetUniformLocation(shader.getId(), uniformName.c_str());
    shader.use();

    glUniform1i(uniformLocation, textureIndex);
}

////////////////////////////////////////////////

void CubeMapFramebuffer::bindTextures() const
{
    for(unsigned int i = 0; i < mTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i]);
    }

}

////////////////////////////////////////////////

void CubeMapFramebuffer::bindTexture(GLenum textureUnit, size_t textureIndex) const
{
    if(textureIndex >= mTextures.size())
        throw std::runtime_error("Index out of bounds.");

    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, mTextures[textureIndex]);
}

////////////////////////////////////////////////

void CubeMapFramebuffer::getPixels(size_t attachmentId, size_t x, size_t y, size_t width, size_t height, void* pixelData) const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentId);

    glReadPixels(x, y, width, height, mTexturePixelFormat, mTextureDataType, pixelData);
}

////////////////////////////////////////////////

size_t CubeMapFramebuffer::getWidth() const
{
    return mImageWidth;
}

////////////////////////////////////////////////

size_t CubeMapFramebuffer::getHeight() const
{
    return mImageHeight;
}
