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
#include "World.hpp"
#include "Window.hpp"
#include "demos/TerrainDemo.hpp"
#include "demos/QuadtreeDemo.hpp"
#include "demos/ParticleSystemDemo.hpp"
#include "demos/MousePickingDemo.hpp"
#include "demos/DynCubeEnvMappingDemo.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// OpenGL
#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
////////////////////////////////////////////////


////////////////////////////////////////////////
// STD - C++ Standard Library
#include <iostream>
////////////////////////////////////////////////

World::World(Window& window)
: mWindow(window)
, mLightPassProgram(ShaderProgram::Id::DEFERRED_RENDER_LIGHT, true, false, true)
, mFramebuffer(window.getWidth(), window.getHeight(), GL_RGBA32F, GL_RGBA, GL_FLOAT, 7)
{
    mFramebuffer.createInternalDepthBuffer();

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer.getFbo());

    GLuint textureIds[7];
    glGenTextures(7, textureIds);
    for(size_t i = 0; i < 7; i++)
        mTextures.push_back(Texture(textureIds[i]));


    for(unsigned int i = 0; i < 7; i++)
    {
        glBindTexture(GL_TEXTURE_2D, mTextures[i].getId());

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window.getWidth(), window.getHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
        mFramebuffer.setAttachmentTexture(i, GL_TEXTURE_2D, mTextures[i].getId(), 0);
    }

    mLightPassProgram.use();
    glUniform1i(ShaderProgram::getCurrentProgramUniformLocation("pos"), 0);
    glUniform1i(ShaderProgram::getCurrentProgramUniformLocation("diffuse"), 1);
    glUniform1i(ShaderProgram::getCurrentProgramUniformLocation("normal"), 2);
    glUniform1i(ShaderProgram::getCurrentProgramUniformLocation("texCoord"), 3);
    glUniform1i(ShaderProgram::getCurrentProgramUniformLocation("ambient"), 5);
    glUniform1i(ShaderProgram::getCurrentProgramUniformLocation("specular"), 6);


    glUniform2f(ShaderProgram::getCurrentProgramUniformLocation("windowSize"), mWindow.getWidth(), mWindow.getHeight());


//    mFramebuffer.bindTextureToShaderUniform(0, "pos", mLightPassProgram);
//    mFramebuffer.bindTextureToShaderUniform(1, "diffuse", mLightPassProgram);
//    mFramebuffer.bindTextureToShaderUniform(2, "normal", mLightPassProgram);
//    mFramebuffer.bindTextureToShaderUniform(3, "texCoord", mLightPassProgram);

    mDemos.push_back(std::unique_ptr<Demo>(new TerrainDemo(mWindow.getView())));
//    mDemos.push_back(std::unique_ptr<Demo>(new QuadtreeDemo(mWindow.getView())));
//    mDemos.push_back(std::unique_ptr<Demo>(new ParticleSystemDemo(mWindow.getView())));
//    mDemos.push_back(std::unique_ptr<Demo>(new MousePickingDemo(mFramebuffer)));
//
//    mDemos.push_back(std::unique_ptr<Demo>(new DynCubeEnvMappingDemo(mFramebuffer, mLightPassProgram)));

}

////////////////////////////////////////////////



void World::handleInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
//                std::cout << "User just pressed down a key!" << std::endl;
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    mWindow.close();
                break;

            case SDL_QUIT:
                mWindow.close();
                break;

            case SDL_MOUSEMOTION:
                //mWindow.rotateView(event.motion.xrel / 100.f, glm::vec3(0.f, 1.f, 0.f));
                //mWindow.rotateView(event.motion.yrel / 100.f, glm::vec3(1.f, 0.f, 0.f));
                mWindow.rotateView(event.motion.xrel / 1000.f, event.motion.yrel / 1000.f);
                break;

            default:
                break;
        }

        for(auto& demo : mDemos)
            demo->handleInput(event);
    }

    const uint8_t* isKeyDown = SDL_GetKeyboardState(nullptr);

    if(isKeyDown[SDL_SCANCODE_A])
    {
        mWindow.moveView(-0.1f, 0.f, 0.f);
//        std::cout << "Moving to the LEFT." << std::endl;
    }
    else if(isKeyDown[SDL_SCANCODE_D])
    {
        mWindow.moveView(0.1f, 0.f, 0.f);
        //std::cout << "Moving to the RIGHT." << std::endl;
    }

    if(isKeyDown[SDL_SCANCODE_W])
    {
        mWindow.moveView(0.f, 0.f, -0.1f);
        //std::cout << "Moving FORWARD." << std::endl;
    }
    else if(isKeyDown[SDL_SCANCODE_S])
    {
        mWindow.moveView(0.f, 0.f, 0.1f);
//        std::cout << "Moving BACKWARD." << std::endl;
    }

    if(isKeyDown[SDL_SCANCODE_SPACE])
    {
        mWindow.moveView(0.f, 0.1f, 0.f);
//        std::cout << "Moving UP." << std::endl;
    }
    else if(isKeyDown[SDL_SCANCODE_LSHIFT])
    {
        mWindow.moveView(0.f, -0.1f, 0.f);
//        std::cout << "Moving DOWN." << std::endl;
    }
}


void World::update()
{
    for(auto& demo : mDemos)
        demo->update();
}




void renderDeferredRenderBuffer(size_t width, size_t height)
{
    const size_t WINDOW_WIDTH = width;
    const size_t WINDOW_HEIGHT = height;
    const size_t HALF_WINDOW_WIDTH = WINDOW_WIDTH / 2;
    const size_t HALF_WINDOW_HEIGHT = WINDOW_HEIGHT / 2;

    glReadBuffer(GL_COLOR_ATTACHMENT0 + 0);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                    0, 0, HALF_WINDOW_WIDTH, HALF_WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glReadBuffer(GL_COLOR_ATTACHMENT0 + 1);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                    0, HALF_WINDOW_HEIGHT, HALF_WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glReadBuffer(GL_COLOR_ATTACHMENT0 + 2);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                    HALF_WINDOW_WIDTH, HALF_WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glReadBuffer(GL_COLOR_ATTACHMENT0 + 3);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                    HALF_WINDOW_WIDTH, 0, WINDOW_WIDTH, HALF_WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}


void World::draw() // const
{
    const Camera& view = mWindow.getView();
    glm::mat4 transform = view.getGlobalTransform();

    mLightPassProgram.use();
    glm::vec3 eye = view.getPosition();
    glUniform3fv(ShaderProgram::getCurrentProgramUniformLocation("eyePosition"), 1, &eye[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer.getFbo());
    {
        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        for(/*const */auto& demo : mDemos)
            demo->drawGeometry(transform);

        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
    }
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer.getFbo());
    {
        for(unsigned int i = 0; i < mTextures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mFramebuffer.getAttachmentImageId(i));
        }

        mLightPassProgram.use();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//        renderDeferredRenderBuffer(mWindow.getWidth(), mWindow.getHeight());
    }
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}


