#include <gal/gal.hpp>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <cassert>
#include <sstream>
#include <type_traits>

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>


namespace gal
{


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

GLuint createShaderProgram()
{
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	static const char* vsSrc = R"EOF(

    #version 130
    in vec3 vertexPosition_modelspace;
    in vec3 normalIn;
    in vec3 uvIn;
    out vec3 uv;
    out vec3 normal;
    uniform mat4 mv;
    uniform mat4 mvp;
    void main()
    {
      gl_Position = mvp * vec4(vertexPosition_modelspace, 1);
      gl_Position /= gl_Position.w;

      normal = (mv * vec4(normalIn, 0)).xyz;
      uv = uvIn;
    }

  )EOF";

  static const char* fsSrc = R"EOF(

    #version 130
    out vec3 color;
    in vec3 uv;
    in vec3 normal;

    uniform sampler2D textureSampler;
    uniform vec3 light_direction;

    void main()
    {
      color = texture(textureSampler, uv.xy).rgb;

      float diffuse = clamp(-dot(light_direction, normal), 0, 1);
      const float ambient = 0.2; 
      float modifier = clamp(ambient + diffuse, 0, 1);
      color *= modifier;
    }

  )EOF";

  compileShader(vs, vsSrc);
  compileShader(fs, fsSrc);

	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);

  glBindAttribLocation(prog, 0, "vertexPosition_modelspace");
  glBindAttribLocation(prog, 1, "normalIn");
  glBindAttribLocation(prog, 2, "uvIn");

	glLinkProgram(prog);

	printProgramLog(prog);

	glDetachShader(prog, vs);
	glDetachShader(prog, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(prog);
	throwGlError("Failed to create shader program");

  return prog;
}

static_assert(std::is_same_v<GLushort, Index>, "Index must be same type as underlying graphics API");

namespace impl
{
  using Id = unsigned int;

  class Mesh
  {
    public:
      Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices);


      void initialize();
      void render();

    private:
      std::vector<Vertex> _vertices;
      std::vector<Index> _indices;
      Id _vao = 0;
      Id _vbo = 0;
      Id _indices_vbo = 0;
      size_t _size;
  };

  class Texture
  {
    public:
      Texture(const char* data, size_t size, size_t width, size_t height, ::gal::Texture::Format format);

      void initialize();
      void bind();

    private:
      std::vector<char> _data;
      size_t _width;
      size_t _height;
      Id _tex = 0;
  };


  static_assert(std::is_same_v<GLuint, Id>, "Id must be same type as underlying graphics API");


  class Renderer
  {
    public:
      static void render(std::vector<Draw>& draws);
      static void render(std::vector<Light>& lights);
  };

  void Renderer::render(std::vector<Draw>& draws)
  {
    for(Draw& draw : draws)
    {
      draw.initialize();
      draw.render();
    }
  }

  void Renderer::render(std::vector<Light>& lights)
  {
    for(Light& light : lights)
    {
      light.render();
    }
  }

  class Buffer
  {
    public:
      Buffer();

      void push(const Light& light);
      void push(const Draw& draw);



      std::vector<Light>& get_lights();
      std::vector<Draw>& get_draws();
      void swap();


    private:
      struct Data
      {
        std::vector<Light> lights;
        std::vector<Draw> draws;
      };
      Data _buffer_1;
      Data _buffer_2;

      Data* _front_buffer;
      Data* _back_buffer;
  };



  Buffer::Buffer()
  : _front_buffer(&_buffer_1)
  , _back_buffer(&_buffer_2)
  {

  }

  void Buffer::push(const Light& light)
  {
    _back_buffer->lights.push_back(light);
  }

  void Buffer::push(const Draw& draw)
  {
    _back_buffer->draws.push_back(draw);
  }

  std::vector<Light>& Buffer::get_lights()
  {
    return _front_buffer->lights;
  }

  std::vector<Draw>& Buffer::get_draws()
  {
    return _front_buffer->draws;
  }



  void Buffer::swap()
  {
    _front_buffer->lights.clear();
    _front_buffer->draws.clear();
    std::swap(_front_buffer, _back_buffer);
  }


  namespace
  {
    Buffer _draw_buffer;
    Viewport _default_viewport;
    GLuint _shader;
  }
    


}


impl::Texture::Texture(const char* data, size_t size, size_t width, size_t height, ::gal::Texture::Format format)
: _width(width)
, _height(height)
{
  assert(format == ::gal::Texture::Format::RGBA);
  _data.assign(data, data + size);
}

void impl::Texture::initialize()
{
  if(_tex != 0)
  {
    return;
  }

  glGenTextures(1, &_tex);
  glBindTexture(GL_TEXTURE_2D, _tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


  glBindTexture(GL_TEXTURE_2D, _tex);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data.data());

  _data.clear();
}

void Draw::initialize()
{
  if(!_has_viewport)
  {
    _viewport = impl::_default_viewport;
  }
  if(_mesh)
  {
    _mesh->initialize();
  }

  for(impl::Texture* texture : _textures)
  {
    texture->initialize();
  }
}

glm::mat4 to_glm(const Transform& transform)
{
  glm::mat4 mat;
  memcpy(&mat[0][0], &transform.data[0][0], sizeof(transform.data)); 
  return mat;
} 

void Draw::render()
{
  for(impl::Texture* texture : _textures)
  {
    texture->bind();
  }

  for(const Transform& transform : _transforms)
  {
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 
                                        1.0f * float(_viewport.width) / float(_viewport.height), 
                                        0.1f, 
                                        10.0f);


    glm::mat4 mv = to_glm(transform);
    glm::mat4 mvp = proj * mv;

    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    GLint uniform_mvp = glGetUniformLocation(program, "mvp");
    GLint uniform_mv = glGetUniformLocation(program, "mv");

    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, &mv[0][0]);
  
    _mesh->render();  
  }
}

void Light::set(Type type)
{
  _type = type;
}

void Light::set(const Transform& transform)
{
  _transform = transform;
}

void Light::draw() const
{
  impl::_draw_buffer.push(*this);
}

void Light::render()
{
  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  GLint uniform_light = glGetUniformLocation(program, "light_direction");

  glm::vec4 direction4 =  to_glm(_transform) * glm::vec4(0, 0, -1, 0);

  glm::vec3 direction = glm::vec3(direction4);
  direction = glm::normalize(direction);

  glUniform3fv(uniform_light, 1, &direction[0]);
}


impl::Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices)
: _vertices(vertices)
, _indices(indices)
{

}

void impl::Mesh::initialize()
{
  if(_vao != 0)
  {
    return;
  }

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(0, sizeof(Position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, sizeof(Normal) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)sizeof(Position));
  glVertexAttribPointer(2, sizeof(Uv) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)sizeof(Position) + sizeof(Normal));
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);


  glGenBuffers(1, &_indices_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices_vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index) * _indices.size(), _indices.data(), GL_STATIC_DRAW);
  
  _size = _indices.size();
  _vertices.clear();
  _indices.clear();
}

void impl::Mesh::render()
{
  glBindVertexArray(_vao);

  static_assert(std::is_same_v<Index, GLushort>, "");
  glDrawElements(GL_TRIANGLE_STRIP, _size, GL_UNSIGNED_SHORT, 0);
}

void impl::Texture::bind()
{
  glBindTexture(GL_TEXTURE_2D, _tex);
}


void initialize()
{
  GLenum glewInitResult = glewInit();
  if (glewInitResult != GLEW_OK)
  {
      throw std::runtime_error("Failed to initialize GLEW");
  }
  std::cout << "[gal] initialized OpenGL with " << glGetString(GL_VERSION) << ", " << glGetString(GL_RENDERER) << std::endl;

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.f, 0.f, 1.f, 1.f);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  impl::_default_viewport.x = viewport[0];
  impl::_default_viewport.y = viewport[1];
  impl::_default_viewport.width = viewport[2];
  impl::_default_viewport.height = viewport[3];

  impl::_shader = createShaderProgram();

  glDisable(GL_CULL_FACE);
}


void render()
{
  static bool is_initialized = false;
  if (!is_initialized)
  {
    initialize();
    is_initialized = true;
  }

  impl::_draw_buffer.swap();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  impl::Renderer::render(impl::_draw_buffer.get_draws());
  impl::Renderer::render(impl::_draw_buffer.get_lights());
}


Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices)
: _mesh(new impl::Mesh(vertices, indices))
{

}


Mesh::~Mesh()
{
  delete _mesh;
}

Texture::Texture(const char* data, size_t size, size_t width, size_t height, Format format)
: _texture(new impl::Texture(data, size, width, height, format))
{

}

Texture::~Texture()
{
  delete _texture;
}

void Draw::set(const Mesh& mesh)
{
  _mesh = mesh._mesh;
}

void Draw::set(const std::vector<Transform>& transforms)
{
  _transforms = transforms;
}

void Draw::set(const std::vector<Texture*>& textures)
{
  _textures.resize(textures.size());

  for(size_t i = 0; i < _textures.size(); i++)
  {
    _textures[i] = textures[i]->_texture;
  }
}

void Draw::set(const Viewport& viewport)
{
  _viewport = viewport;
  _has_viewport = true;
}

void Draw::draw() const
{
  impl::_draw_buffer.push(*this);
}



}