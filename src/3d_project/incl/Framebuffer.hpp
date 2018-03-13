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

#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

////////////////////////////////////////////////
// Internal headers
class ShaderProgram;
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL
#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
////////////////////////////////////////////////


////////////////////////////////////////////////
// STD - C++ Standard Library
#include <vector>
#include <string>
////////////////////////////////////////////////

class Framebuffer
{
    public:
        Framebuffer(size_t width, size_t height, GLint textureInternalFormat, GLenum texturePixelFormat, GLenum textureDataType, unsigned int numTextures);
        ~Framebuffer();

        GLuint getFbo() const;

        void setCubeMapTexture(GLuint cubeMapId, GLenum cubeMapTarget, GLenum attachment = GL_COLOR_ATTACHMENT0);
        void setNumTextures(unsigned int numTextures);

        GLuint getTextureId(unsigned int i) const;
        size_t getNumTextures() const;
        void bindTextureToShaderUniform(unsigned int textureIndex, std::string uniformName, const ShaderProgram& shader) const;
        void bindTextures() const;
        void bindTexture(GLenum textureUnit, size_t textureIndex) const;

        void createInternalDepthBuffer();

        void getPixels(size_t attachmentId, size_t x, size_t y, size_t width, size_t height, void* pixelData) const;

        size_t getWidth() const;
        size_t getHeight() const;

        void setAttachmentTexture(size_t index, GLenum textureType, GLuint textureId, GLint textureLayer) const;
        GLuint getAttachmentImageId(size_t index) const;

        std::string getStatus() const;

    private:


    private:
        GLuint mFbo = 0;
        GLuint mDepthTexture = 0;
        GLuint mStencilTexture = 0;

        size_t mImageWidth;
        size_t mImageHeight;
        GLint mTextureInternalFormat;
        GLenum mTexturePixelFormat;
        GLenum mTextureDataType;

        size_t mNumTextures;
};


#endif // FRAMEBUFFER_HPP
