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

#ifndef WORLD_HPP
#define WORLD_HPP

////////////////////////////////////////////////
// Internal headers
class Window;
#include "Demo.hpp"
#include "Framebuffer.hpp"
#include "ShaderProgram.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <memory>
////////////////////////////////////////////////


class Texture
{
    public:
        Texture(GLuint id)
        : M_ID(id)
        {

        }

        ~Texture()
        {
            glDeleteTextures(1, &M_ID);
        }

        GLuint getId() const
        {
            return M_ID;
        }

    private:
        const GLuint M_ID;

};

class World
{
    public:
        World(Window& window);

        void handleInput();
        void update();
        void draw(); // const

    private:
        Window& mWindow;
        Framebuffer mFramebuffer;
        ShaderProgram mLightPassProgram;
        std::vector<std::unique_ptr<Demo>> mDemos;
        std::vector<Texture> mTextures;
};

#endif // WORLD_HPP
