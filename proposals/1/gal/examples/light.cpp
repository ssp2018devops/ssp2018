
#include <gal/gal.hpp>

#include <vector>
#include <iostream>

#include <SDL.h>


#include <glm/gtc/matrix_transform.hpp>

SDL_Window* openWindow();
SDL_GLContext createGlContext(SDL_Window* window);
void close(SDL_Window* window, SDL_GLContext context);

int main()
{ 
  
  
  // Mesh data of a cube.
  gal::MeshData mesh_data;
  mesh_data.vertices = 
  {
    // {{position}, {normal}, {uv}}

    // front
    {{-1, -1, 1}, {0, 0, 1}, {0, 0}},
    {{1, -1, 1}, {0, 0, 1}, {1, 0}},
    {{-1, 1, 1}, {0, 0, 1}, {0, 1}},
    {{1, 1, 1}, {0, 0, 1}, {1, 1}},

    // right
    {{1, -1, 1}, {1, 0, 0}, {0, 0}},
    {{1, -1, -1}, {1, 0, 0}, {1, 0}},    
    {{1, 1, 1}, {1, 0, 0}, {0, 1}},
    {{1, 1, -1}, {1, 0, 0}, {1, 1}},

    // back
    {{1, -1, -1}, {0, 0, -1}, {0, 0}},
    {{-1, -1, -1}, {0, 0, -1}, {1, 0}},
    {{1, 1, -1}, {0, 0, -1}, {0, 1}},
    {{-1, 1, -1}, {0, 0, -1}, {1, 1}},

    // left
    {{-1, -1, -1}, {-1, 0, 0}, {0, 0}},
    {{-1, -1, 1}, {-1, 0, 0}, {1, 0}},
    {{-1, 1, -1}, {-1, 0, 0}, {0, 1}},
    {{-1, 1, 1}, {-1, 0, 0}, {1, 1}},

    // bot
    {{-1, -1, -1}, {0, -1, 0}, {0, 0}},
    {{1, -1, -1}, {0, -1, 0}, {1, 0}},
    {{-1, -1, 1}, {0, -1, 0}, {0, 1}},
    {{1, -1, 1}, {0, -1, 0}, {1, 1}},

    // top
    {{-1, 1, 1}, {0, 1, 0}, {0, 0}},
    {{1, 1, 1}, {0, 1, 0}, {1, 0}}, 
    {{-1, 1, -1}, {0, 1, 0}, {0, 1}},
    {{1, 1, -1}, {0, 1, 0}, {1, 1}},
  };

  mesh_data.indices = 
  {
    // front
    0, 1, 2, 3, 3,

    // right
    4, 4, 5, 6, 7, 7,

    // back
    8, 8, 9, 10, 11, 11,

    // left
    12, 12, 13, 14, 15, 15,

    // bot
    16, 16, 17, 18, 19, 19,

    // top
    20, 20, 21, 22, 23, 23,
  };

  // Create mesh.
  gal::Mesh mesh(mesh_data);

  // Create draw.
  gal::Draw draw;

  // Add mesh to draw.
  draw.set(mesh);

  // Blue texture data.
  gal::TextureData texture_data;
  texture_data.format = gal::TextureData::Format::RGBA;
  texture_data.width = 1;
  texture_data.height = 1;
  unsigned char blue[4] = {100, 100, 255, 255};
  texture_data.data = (const char*)blue;

  // Create texture.
  gal::Texture texture(texture_data);

  // Add texture to draw.
  draw.set({&texture});

  // Create transform matrices.
  glm::vec3 eye(0, 2, 5);
  glm::vec3 center(0, 0, 0);
  glm::vec3 up(0, 1, 0);
  glm::mat4 view_transform = glm::lookAt(eye, center, up);
  glm::mat4 model_transform(1.f);



  SDL_Window* window = openWindow();
  SDL_GLContext context = createGlContext(window);
  bool isRunning = true;
  while(isRunning)
  {
      SDL_PumpEvents();
      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
          switch(event.type)
          {
              case SDL_KEYDOWN:
                  if(event.key.keysym.sym == SDLK_ESCAPE)
                  {
                      isRunning = false;
                  }
                  break;

              case SDL_QUIT:
                  isRunning = false;
                  break;

              default:
                  break;
          }
      }

      // Rotate cube.
      model_transform = glm::rotate(model_transform, 0.01f, glm::vec3(0, 1, 0));
      glm::mat4 transform = view_transform * model_transform;

      // Set transform.
      draw.set({&transform[0][0]});

      // Draw the mesh.
      draw.draw();

      // Render results.
      gal::render();

      SDL_GL_SwapWindow(window);
  }

  close(window, context);

  return 0;
}


void close(SDL_Window* window, SDL_GLContext context)
{
  SDL_DestroyWindow(window);
  SDL_GL_DeleteContext(context);
  SDL_Quit();
}

SDL_Window* openWindow()
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cout << "Failed to initialize SDL." << std::endl;
    throw std::runtime_error(SDL_GetError());
  }

  int width = 400;
  int height = 400;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_Window* window = SDL_CreateWindow
  (
    "An SDL2 window",                  // window title
    SDL_WINDOWPOS_UNDEFINED,           // initial x position
    SDL_WINDOWPOS_UNDEFINED,           // initial y position
    width,                               // width, in pixels
    height,                               // height, in pixels
    SDL_WINDOW_OPENGL                  // flags - see below
  );


  if(window == NULL)
  {
    std::cout << "Failed to create SDL window." << std::endl;
    throw std::runtime_error(SDL_GetError());
  }



  return window;
}

SDL_GLContext createGlContext(SDL_Window* window)
{
  SDL_GLContext context = SDL_GL_CreateContext(window);
  if(context == NULL)
  {
    std::cout << "Failed to create OpenGL context." << std::endl;
    throw std::runtime_error(SDL_GetError());
  }

  if(SDL_GL_SetSwapInterval(1) != 0)
  {
    throw std::runtime_error("Failed to set SDL swap interval.");
  }
  return context;
}
