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
#include "Framebuffer.hpp"
#include "ShaderProgram.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <stdexcept>
#include <iostream>
#include <sstream>
////////////////////////////////////////////////


Framebuffer::Framebuffer(size_t width, size_t height, GLint textureInternalFormat, GLenum texturePixelFormat, GLenum textureDataType, unsigned int numTextures)
: mImageWidth(width)
, mImageHeight(height)
, mTextureInternalFormat(textureInternalFormat)
, mTexturePixelFormat(texturePixelFormat)
, mTextureDataType(textureDataType)
, mNumTextures(numTextures)
{
    if(numTextures < 1)
        throw std::runtime_error("A Framebuffer must contain at least one texture.");

    glGenFramebuffers(1, &mFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

    setNumTextures(numTextures);
}

////////////////////////////////////////////////

Framebuffer::~Framebuffer()
{
    if(mDepthTexture)
        glDeleteTextures(1, &mDepthTexture);

    if(mStencilTexture)
        glDeleteTextures(1, &mStencilTexture);

    glDeleteFramebuffers(1, &mFbo);
}

////////////////////////////////////////////////

GLuint Framebuffer::getFbo() const
{
    return mFbo;
}

////////////////////////////////////////////////

void Framebuffer::setCubeMapTexture(GLuint cubeMapId, GLenum cubeMapTarget, GLenum attachment)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, cubeMapTarget, cubeMapId, 0);
    glDrawBuffers(1, &attachment);
}

////////////////////////////////////////////////

void Framebuffer::setNumTextures(unsigned int numTextures)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

    GLenum* textureAttachmentTypes = new GLenum[numTextures];
    for(unsigned int i = 0; i < numTextures; i++)
        textureAttachmentTypes[i] = GL_COLOR_ATTACHMENT0 + i;

    glDrawBuffers(numTextures, textureAttachmentTypes);

    delete[] textureAttachmentTypes;

    mNumTextures = numTextures;
}

////////////////////////////////////////////////

void Framebuffer::createInternalDepthBuffer()
{
    glGenTextures(1, &mDepthTexture);
    glBindTexture(GL_TEXTURE_2D, mDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, mImageWidth, mImageHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

////////////////////////////////////////////////

size_t Framebuffer::getNumTextures() const
{
    return mNumTextures;
}

////////////////////////////////////////////////

void Framebuffer::getPixels(size_t attachmentId, size_t x, size_t y, size_t width, size_t height, void* pixelData) const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentId);

    glReadPixels(x, y, width, height, mTexturePixelFormat, mTextureDataType, pixelData);
}

////////////////////////////////////////////////

size_t Framebuffer::getWidth() const
{
    return mImageWidth;
}

////////////////////////////////////////////////

size_t Framebuffer::getHeight() const
{
    return mImageHeight;
}

std::string Framebuffer::getStatus() const
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::stringstream stream;
        stream << "FB error (" << status << ")";
        return stream.str();
    }
    else
        return "OK";
}

void Framebuffer::setAttachmentTexture(size_t index, GLenum textureType, GLuint textureId, GLint textureLayer) const
{
    if(index >= mNumTextures)
        throw std::runtime_error("Index out of bounds.");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureType, textureId, textureLayer);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GLuint Framebuffer::getAttachmentImageId(size_t index) const
{
    if(index >= mNumTextures)
        throw std::runtime_error("Index out of bounds.");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

    GLint id;
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &id);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return id;

}
