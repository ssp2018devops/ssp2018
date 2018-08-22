
#include <gal/gal.hpp>

#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>

#include <SDL.h>

#include <glm/glm.hpp>


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

  if(SDL_GL_SetSwapInterval(0) != 0)
  {
    throw std::runtime_error("Failed to set SDL swap interval.");
  }
  return context;
}


struct Pixel
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
};


std::vector<Pixel> generatePixels(size_t count)
{

	std::vector<Pixel> pixels(count);

  std::random_device rnd_device;
  std::mt19937 mersenne(rnd_device());
  std::uniform_int_distribution<unsigned char> dist(0, 255);

  std::generate(pixels.begin(), pixels.end(), [&dist, &mersenne]() { return Pixel({dist(mersenne), dist(mersenne), dist(mersenne)}); });
  return pixels;
}





int main()
{
  SDL_Window* window = openWindow();
  SDL_GLContext context = createGlContext(window);

  // Initialize GAL after creation of rendering context.
  gal::initialize();

  // Create mesh object.
  gal::Mesh mesh;

  // Add positions to mesh.
  gal::PositionBuffer position_buffer;
  mesh.set(position_buffer);
  std::vector<gal::Position> positions = 
  {
    {-0.5f,  -0.5f,  0.0f},
    {0.5f,   -0.5f,  0.0f},
    {-0.5f,  0.5f,   0.0f},
    {0.5f,   0.5f,   0.0f}
  };
  position_buffer.set(positions.data(), positions.size());

  // Add UVs to mesh.
  gal::UvBuffer uv_buffer;
  mesh.set(uv_buffer);
  std::vector<gal::Uv> uvs = 
  { 
    {0.f, 0.f},
    {1.f, 0.f},
    {0.f, 1.f},
    {1.f, 1.f}
  };
  uv_buffer.set(uvs.data(), uvs.size());

  // Create draw object.
  gal::Draw draw;

  // Add mesh to draw object.
  draw.set(mesh);

  // Add texture to draw object.
  gal::TextureBuffer texture_buffer;
  draw.set(texture_buffer);

  gal::Texture tex;
  tex.format = gal::Texture::Format::RGBA;
  tex.width = 32;
  tex.height = 32;

  std::vector<Pixel> pixels = generatePixels(tex.width * tex.height);
  tex.data = (const char*)pixels.data();
  texture_buffer.set(&tex, 1);


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

      // Draw the mesh.
      draw.draw();

      // Render results.
      gal::render();


      SDL_GL_SwapWindow(window);
  }

  close(window, context);

  return 0;
}

