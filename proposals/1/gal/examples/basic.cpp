
#include <gal/gal.hpp>

#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>

#include <SDL.h>

#include <GL/glew.h>

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
  std::cout << glGetString(GL_VERSION) << std::endl;
  std::cout << glGetString(GL_RENDERER) << std::endl;
  if(context == NULL)
  {
    std::cout << "Failed to create OpenGL context." << std::endl;
    throw std::runtime_error(SDL_GetError());
  }

    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
    {
        throw std::runtime_error("Failed to initialize GLEW");
    }

//  glEnable(GL_DEPTH_TEST);
//  glDepthFunc(GL_LESS);
  glDisable(GL_DEPTH_TEST);

  int width = 0;
  int height = 0;
  SDL_GetWindowSize(window, &width, &height);
  glClearColor(1.f, 0.f, 1.f, 1.f);
  glViewport(0, 0, width, height);

  if(SDL_GL_SetSwapInterval(0) != 0)
  {
    throw std::runtime_error("Failed to set SDL swap interval.");
  }
  return context;
}

void printShaderLog(GLuint shader)
{
  int logLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength > 0)
  {
		std::vector<char> msg(logLength + 1);
		glGetShaderInfoLog(shader, logLength, NULL, &msg[0]);
		std::cout << msg.data() << std::endl;
	}
}

void printProgramLog(GLuint prog)
{
  int logLength = 0;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength > 0)
  {
		std::vector<char> msg(logLength + 1);
		glGetProgramInfoLog(prog, logLength, NULL, &msg[0]);
		std::cout << msg.data() << std::endl;
	}
}

void compileShader(GLuint shader, const char* src)
{
  glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
  printShaderLog(shader);
}

void throwGlError(const char* msg)
{
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    std::stringstream sstream;
    sstream << "GL ERROR " << error << ": \"" << msg << "\"" << std::endl;
    throw std::runtime_error(sstream.str());
  }
}

void createShaderProgram()
{
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	static const char* vsSrc = R"EOF(

    #version 130
    in vec3 vertexPosition_modelspace;
    in vec2 uvIn;
    out vec2 uv;
    void main()
    {
      gl_Position.xyz = vertexPosition_modelspace;
      gl_Position.w = 1.0;
      uv = uvIn;
    }

  )EOF";

  static const char* fsSrc = R"EOF(

    #version 130
    out vec3 color;
    in vec2 uv;

    uniform sampler2D textureSampler;

    void main()
    {
      color = texture(textureSampler, uv).rgb;
    }

  )EOF";

  compileShader(vs, vsSrc);
  compileShader(fs, fsSrc);

	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);

  glBindAttribLocation(prog, 0, "vertexPosition_modelspace");
  glBindAttribLocation(prog, 1, "uvIn");

	glLinkProgram(prog);

	printProgramLog(prog);

	glDetachShader(prog, vs);
	glDetachShader(prog, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(prog);
	throwGlError("Failed to create shader program");
}

GLuint createVao(const std::vector<GLfloat>& data)
{
  GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (char*)(sizeof(GLfloat) * 3));

  throwGlError("Failed to create square VAO");
  return vao;
}

GLuint createSquareVao()
{
  return createVao
  ({
    -0.5f,  -0.5f,  0.0f, 0.f, 0.f,
    0.5f,   -0.5f,  0.0f, 1.f, 0.f,
    -0.5f,  0.5f,   0.0f, 0.f, 1.f,
    0.5f,   0.5f,   0.0f, 1.f, 1.f
  });
}

GLuint createRectangleVao(float uvTop, float uvHeight)
{
  float uvBot = uvTop - uvHeight;
  return createVao
  ({
    -1.f,  -1.f,  0.0f, 0.f, uvBot,
    1.f,   -1.f,  0.0f, 1.f, uvBot,
    -1.f,  1.f,   0.0f, 0.f, uvTop,
    1.f,   1.f,   0.0f, 1.f, uvTop
  });
}


void renderSquareVao()
{
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  throwGlError("Failed to render square VAO");
}

void printUsage()
{
  std::cout << "./impl <frame_count> <texture_resolution> <texture_count> <use_atlas> <out_path>" << std::endl;
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
//  for(Pixel& p : pixels)
//  {
//    p = {dist(mersenne), dist(mersenne), dist(mersenne)};
//  }

  return pixels;
}

GLuint createTexture(size_t resolution)
{
  std::vector<Pixel> pixels = generatePixels(resolution * resolution);

  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution, resolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  throwGlError("Failed to create texture.");
  return tex;
}


int main()
{
    gal::initialize();

    gal::Mesh* mesh = gal::create_mesh();
    
    gal::PositionBuffer* position_buffer = gal::create_position_buffer();
    gal::NormalBuffer* normal_buffer = gal::create_normal_buffer();
    gal::UvBuffer* uv_buffer = gal::create_uv_buffer();
    
    gal::set(mesh, position_buffer);
    gal::set(mesh, normal_buffer);
    gal::set(mesh, uv_buffer);

    std::vector<gal::Position> positions = { /* yada yada yada */ };
    std::vector<gal::Normal> normals = { /* yada yada yada */ };
    std::vector<gal::Uv> uvs = { /* yada yada yada */ };

    gal::set(position_buffer, positions.data(), positions.size());
    gal::set(normal_buffer, normals.data(), normals.size());
    gal::set(uv_buffer, uvs.data(), uvs.size());




    gal::Draw* draw = gal::create_draw();

    gal::TransformBuffer* transform_buffer = gal::create_transform_buffer();
    gal::TextureBuffer* texture_buffer = gal::create_texture_buffer();

    gal::set(draw, mesh);
    gal::set(draw, transform_buffer);
    gal::set(draw, texture_buffer);

    std::vector<gal::Transform> transforms = { /* yada yada yada */ };
    std::vector<gal::Texture> textures = { /* yada yada yada */ };

    gal::set(transform_buffer, transforms.data(), transforms.size());
    gal::set(texture_buffer, textures.data(), textures.size());

    gal::draw(draw);


    SDL_Window* window = openWindow();
    SDL_GLContext context = createGlContext(window);

	createShaderProgram();


    GLuint vao = createSquareVao();
    GLuint tex = createTexture(32);
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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, tex);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



        throwGlError("Main loop");
        SDL_GL_SwapWindow(window);
        
    }

    close(window, context);

    return 0;
}

