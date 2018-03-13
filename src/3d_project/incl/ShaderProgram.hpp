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

#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <map>
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL
#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
////////////////////////////////////////////////

class ShaderProgram
{
    public:
        enum class Id : char
        {
            FORWARD_RENDER,
            DEFERRED_RENDER,
            DEFERRED_RENDER_LIGHT,
            TERRAIN,
            PARTICLE_SYSTEM,
            SPHERE,
            LINESTRIP,
            CUBE_MAP,
        };

        ShaderProgram(Id id, bool useVertexShader = true, bool useGeometryShader = true, bool useFragmentShader = true);
        ~ShaderProgram();

        void use() const;
        GLuint getId() const;
        void reload();

        void enableVertexShader(bool flag);
        void enableGeometryShader(bool flag);
        void enableFragmentShader(bool flag);


        static GLint getCurrentProgramId();
        static GLint getCurrentProgramAttribLocation(std::string attribute);
        static GLint getCurrentProgramUniformLocation(std::string uniform);

    private:
        void loadProgram();
        void deleteProgram();
        void deleteShader(GLuint shaderId);

        static GLuint compileShader(std::string filePath, GLenum shaderType);

        static void getShaderProgramFilePaths(Id id, std::string& vertex, std::string& geometry, std::string& fragment);
        static void printShaderInfoLog(GLuint obj);
        static void printProgramInfoLog(GLuint obj);

    private:
        const Id ID;
        GLuint mShaderProgramId         = 0;
        bool mIsVertexShaderEnabled     = false;
        bool mIsGeometryShaderEnabled   = false;
        bool mIsFragmentShaderEnabled   = false;

        static const std::map<Id, std::string> idToPathName;
};

#endif // SHADER_PROGRAM_HPP
