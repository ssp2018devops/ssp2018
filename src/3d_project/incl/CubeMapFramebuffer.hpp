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

#ifndef CUBE_MAP_FRAMEBUFFER_HPP
#define CUBE_MAP_FRAMEBUFFER_HPP

////////////////////////////////////////////////
// Internal headers
class ShaderProgram;
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL
#include "GL/glew.h"
//#define GL_GLEXT_PROTOTYPES
//#include "GL/gl.h"
////////////////////////////////////////////////


////////////////////////////////////////////////
// STD - C++ Standard Library
#include <vector>
#include <string>
////////////////////////////////////////////////

class CubeMapFramebuffer
{
    public:
        CubeMapFramebuffer(size_t size, GLint textureInternalFormat, GLenum texturePixelFormat, GLenum textureDataType, unsigned int numTextures = 1);
        ~CubeMapFramebuffer();

        GLuint getFbo() const;

        void setNumTextures(unsigned int numTextures);

        GLuint getTextureId(unsigned int i) const;
        size_t getNumTextures() const;
        void bindTextureToShaderUniform(unsigned int textureIndex, std::string uniformName, const ShaderProgram& shader) const;
        void bindTextures() const;
        void bindTexture(GLenum textureUnit, size_t textureIndex) const;

        void getPixels(size_t attachmentId, size_t x, size_t y, size_t width, size_t height, void* pixelData) const;

        size_t getWidth() const;
        size_t getHeight() const;

    private:


    private:
        GLuint mFbo = 0;
        std::vector<GLuint> mTextures;

        size_t mSize;
};


#endif // CUBE_MAP_FRAMEBUFFER_HPP
