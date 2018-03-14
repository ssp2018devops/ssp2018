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

#ifndef TERRAIN_HPP
#define TERRAIN_HPP


////////////////////////////////////////////////
// Internal headers
#include "GeometricShape.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL
#include "GL/glew.h"
//#define GL_GLEXT_PROTOTYPES
//#include "GL/gl.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// Simple DirectMedia Layer (SDL)
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
////////////////////////////////////////////////


////////////////////////////////////////////////
// STD - C++ Standard Library
#include <string>
////////////////////////////////////////////////

class Terrain : public GeometricShape
{
    public:
		Terrain(std::string heightmapPath, std::string texturePath);
		~Terrain();
		void draw(GLuint mvpHandle, glm::mat4 transform) const;

		float getHeight(float x, float z) const;

    private:
        unsigned char* generateMesh(const unsigned char* heightmapPixels, size_t heightmapHeight) const;
        void loadHeightmap(std::string heightmapPath);
        void loadTexture(std::string texturePath);
    private:
        size_t mNumVertices;
        size_t mImageWidth;
        GLuint mVao;
		GLuint mVbo;

        GLuint mTextureId;
        GLuint mNormalMapId;

		GLint mImageWidthHandle;

		unsigned char* mHeightmap;
		size_t mImageSize;

};


#endif // TERRAIN_HPP
