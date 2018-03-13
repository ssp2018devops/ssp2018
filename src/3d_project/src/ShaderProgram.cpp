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
#include "ShaderProgram.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL
#include "GL/glext.h"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <fstream>
#include <stdexcept>
#include <iostream>
////////////////////////////////////////////////

const std::map<ShaderProgram::Id, std::string> ShaderProgram::idToPathName =
{
    {Id::FORWARD_RENDER, "forward_render"},
    {Id::DEFERRED_RENDER, "deferred_render"},
    {Id::DEFERRED_RENDER_LIGHT, "deferred_render_light"},
    {Id::TERRAIN, "terrain"},
    {Id::PARTICLE_SYSTEM, "particle_system"},
    {Id::SPHERE, "sphere"},
    {Id::LINESTRIP, "linestrip"},
    {Id::CUBE_MAP, "cube_map"},
};

ShaderProgram::ShaderProgram(Id id, bool useVertexShader, bool useGeometryShader, bool useFragmentShader)
: ID(id)
{
    enableVertexShader(useVertexShader);
    enableGeometryShader(useGeometryShader);
    enableFragmentShader(useFragmentShader);

    loadProgram();
}

ShaderProgram::~ShaderProgram()
{
    deleteProgram();
}


void ShaderProgram::use() const
{
    glUseProgram(mShaderProgramId);
}

GLuint ShaderProgram::getId() const
{
    return mShaderProgramId;
}

void ShaderProgram::reload()
{
    bool isIdInUse = (getCurrentProgramId() == mShaderProgramId);

    deleteProgram();
    loadProgram();

    if(isIdInUse)
        glUseProgram(mShaderProgramId);
}

void ShaderProgram::deleteProgram()
{
    if(mShaderProgramId == 0)
        return;

    if(getCurrentProgramId() == mShaderProgramId)
        glUseProgram(0);

    glDeleteProgram(mShaderProgramId);
}

void ShaderProgram::enableVertexShader(bool flag)
{
    mIsVertexShaderEnabled = flag;
}

void ShaderProgram::enableGeometryShader(bool flag)
{
    mIsGeometryShaderEnabled = flag;
}

void ShaderProgram::enableFragmentShader(bool flag)
{
    mIsFragmentShaderEnabled = flag;
}

GLint ShaderProgram::getCurrentProgramId()
{
    GLint shaderProgramId;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgramId);
    return shaderProgramId;
}

GLint ShaderProgram::getCurrentProgramAttribLocation(std::string attribute)
{
    return glGetAttribLocation(getCurrentProgramId(), attribute.c_str());
}

GLint ShaderProgram::getCurrentProgramUniformLocation(std::string uniform)
{
    return glGetUniformLocation(getCurrentProgramId(), uniform.c_str());
}

GLuint ShaderProgram::compileShader(std::string filePath, GLenum shaderType)
{
    std::ifstream shaderFile(filePath);
    if(shaderFile)
    {
        GLuint shaderId = glCreateShader(shaderType);

        std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
        const char* shaderTextPtr = shaderText.c_str();
        glShaderSource(shaderId, 1, &shaderTextPtr, nullptr);
        glCompileShader(shaderId);

        printShaderInfoLog(shaderId);

        shaderFile.close();
        return shaderId;
    }
    else
    {
        shaderFile.close();
        return 0;
    }

}

void ShaderProgram::loadProgram()
{
    std::string vertexFile;
    std::string geometryFile;
    std::string fragmentFile;
    getShaderProgramFilePaths(ID, vertexFile, geometryFile, fragmentFile);

    std::cout << "Loading program: " << ShaderProgram::idToPathName.at(ID) << std::endl;

    mShaderProgramId = glCreateProgram();

    GLuint vertexShader = 0;
    if(mIsVertexShaderEnabled)
    {
        vertexShader = compileShader(vertexFile, GL_VERTEX_SHADER);
        if(!vertexShader)
            std::cout << "Could not compile vertex shader." << std::endl;
        else
            glAttachShader(mShaderProgramId, vertexShader);
    }

    GLuint geometryShader = 0;
    if(mIsGeometryShaderEnabled)
    {
        geometryShader = compileShader(geometryFile, GL_GEOMETRY_SHADER_EXT);
        if(!geometryShader)
            std::cout << "Could not compile geometry shader." << std::endl;
        else
            glAttachShader(mShaderProgramId, geometryShader);
    }

    GLuint fragmentShader = 0;
    if(mIsFragmentShaderEnabled)
    {
        fragmentShader = compileShader(fragmentFile, GL_FRAGMENT_SHADER);
        if(!fragmentShader)
            std::cout << "Could not compile fragment shader." << std::endl;
        else
            glAttachShader(mShaderProgramId, fragmentShader);
    }

    glLinkProgram(mShaderProgramId);

    printProgramInfoLog(mShaderProgramId);

    if(vertexShader)
        deleteShader(vertexShader);

    if(geometryShader)
        deleteShader(geometryShader);

    if(fragmentShader)
        deleteShader(fragmentShader);

}

void ShaderProgram::deleteShader(GLuint shaderId)
{
    if(shaderId != 0)
    {
        glDetachShader(mShaderProgramId, shaderId);
        glDeleteShader(shaderId);
    }
}

void ShaderProgram::getShaderProgramFilePaths(Id id, std::string& vertex, std::string& geometry, std::string& fragment)
{
    auto found = idToPathName.find(id);
    const std::string shadersDir = "shaders/";
    if(found != idToPathName.end())
    {
        vertex      = shadersDir + found->second + "_vs.glsl";
        geometry    = shadersDir + found->second + "_gs.glsl";
        fragment    = shadersDir + found->second + "_fs.glsl";
    }
    else
        throw std::runtime_error("Unsupported shader program ID. Cannot fetch file path.");

}

void ShaderProgram::printShaderInfoLog(GLuint obj)
{
    int infoLogLength = 0;
    int charsWritten = 0;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 1)
    {
        char* infoLog = new char[infoLogLength];
        glGetShaderInfoLog(obj, infoLogLength, &charsWritten, infoLog);
        std::cout << "printShaderInfoLog: " << infoLog << std::endl;
        delete[] infoLog;
    }
    else
        std::cout << "Shader Info Log: OK" << std::endl;
}

void ShaderProgram::printProgramInfoLog(GLuint obj)
{
    int infoLogLength = 0;
    int charsWritten = 0;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 1)
    {
        char* infoLog = new char[infoLogLength];
        glGetProgramInfoLog(obj, infoLogLength, &charsWritten, infoLog);
        std::cout << "printProgramInfoLog: " << infoLog << std::endl;
        delete[] infoLog;
    }
    else
        std::cout << "Program Info Log: OK" << std::endl;
}
