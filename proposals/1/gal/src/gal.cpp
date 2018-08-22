#include <gal/gal.hpp>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <cassert>
#include <sstream>
#include <type_traits>

#include <GL/glew.h>



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

  return prog;
}


namespace impl
{

  static_assert(std::is_same_v<GLuint, Id>, "Id must be same type as underlying graphics API");


  class Renderer
  {
    public:
      static void render(std::vector<Draw*>& draws);
  };

  void Renderer::render(std::vector<Draw*>& draws)
  {
    for(Draw* draw : draws)
    {
      draw->update();
      draw->render();
    }
  }

  class Buffer
  {
    public:
      Buffer();

      std::vector<Draw*>& get_front();
      std::vector<Draw*>& get_back();
      void swap();


    private:
      std::vector<Draw*> _buffer_1;
      std::vector<Draw*> _buffer_2;

      std::vector<Draw*>* _front_buffer;
      std::vector<Draw*>* _back_buffer;
  };



  Buffer::Buffer()
  : _front_buffer(&_buffer_1)
  , _back_buffer(&_buffer_2)
  {

  }

  std::vector<Draw*>& Buffer::get_front()
  {
    return *_front_buffer;
  }

  std::vector<Draw*>& Buffer::get_back()
  {
    return *_back_buffer;
  }

  void Buffer::swap()
  {
    _front_buffer->clear();
    std::swap(_front_buffer, _back_buffer);
  }


  namespace
  {
    Buffer _draw_buffer;
    Viewport _default_viewport;
    GLuint _shader;
  }
    


}


Mesh::Mesh()
{

}

Mesh::~Mesh()
{ 
}


void Mesh::set(const PositionBuffer& buffer)
{
  _positions = (PositionBuffer*)&buffer;
  _is_dirty = true;
}

void Mesh::set(const UvBuffer& buffer)
{
  _uvs = (UvBuffer*)&buffer;
  _is_dirty = true;
}


Draw::Draw()
: _viewport(impl::_default_viewport)
{

}


void Draw::set(const Mesh& mesh)
{
  _mesh = (Mesh*)&mesh;
}

void Draw::set(const TransformBuffer& buffer)
{
  _transforms = (TransformBuffer*)&buffer;
}

void Draw::set(const TextureBuffer& buffer)
{
  _textures = (TextureBuffer*)&buffer;
}

void Draw::set(const Viewport& viewport)
{
  _viewport = viewport;
}


void Draw::draw() const
{
  impl::_draw_buffer.get_back().push_back((Draw*)this);
}


void PositionBuffer::set(const Position* data, size_t count)
{
  _positions.assign(data, data + count);
}

void NormalBuffer::set(const Normal* data, size_t count)
{
  _normals.assign(data, data + count);
}

void UvBuffer::set(const Uv* data, size_t count)
{
  _uvs.assign(data, data + count);
}

void TransparencyBuffer::set(const Transparency* data, size_t count)
{
  _transparencies.assign(data, data + count);
}

void TransformBuffer::set(const Transform* data, size_t count)
{
  _transforms.assign(data, data + count);
}

void TextureBuffer::set(const Texture* data, size_t count)
{
  _textures.resize(count);
  for(size_t i = 0; i < count; i++)
  {
    InternalTexture& tex = _textures[i];
    tex.header = data[i];
    assert(tex.header.format == Texture::Format::RGBA);
    tex.data.assign(tex.header.data, tex.header.data + tex.header.width * tex.header.height * 4);
  }
}


Position* PositionBuffer::stream(size_t count)
{
  return nullptr;
}

Normal* NormalBuffer::stream(size_t count)
{
  return nullptr;
}

Uv* UvBuffer::stream(size_t count)
{
  return nullptr;
}

Transparency* TransparencyBuffer::stream(size_t count)
{
  return nullptr;
}

Transform* TransformBuffer::stream(size_t count)
{
  return nullptr;
}

Texture* TextureBuffer::stream(size_t count)
{
  return nullptr;
}



PositionBuffer::PositionBuffer()
{

}

PositionBuffer::~PositionBuffer()
{
  
}


UvBuffer::UvBuffer()
{

}

UvBuffer::~UvBuffer()
{
  
}


TextureBuffer::TextureBuffer()
{

}

TextureBuffer::~TextureBuffer()
{
  
}



void Draw::update()
{
  _mesh->update();
  _textures->update();
}

void Draw::render()
{
  _textures->bind();
  _mesh->render();  
}

void Mesh::update()
{
  if(_vao == 0)
  {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
  }

  if(_is_dirty)
  {
    _is_dirty = false;
    
    _positions->update();
    _uvs->update();
    
    glBindVertexArray(_vao);

    _positions->bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Position), 0);

    _uvs->bind();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Uv), 0);
  }
}

void Mesh::render()
{
  glBindVertexArray(_vao);
  _positions->render();
}

void PositionBuffer::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void PositionBuffer::render()
{
  glDrawArrays(GL_TRIANGLE_STRIP, 0, _size);
}

void UvBuffer::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void TextureBuffer::bind()
{
  glBindTexture(GL_TEXTURE_2D, _tex);
}

void TransformBuffer::update()
{
  
}

void TextureBuffer::update()
{
  if(_tex == 0)
  {
    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  }

  if(!_textures[0].data.empty())
  {
    InternalTexture& tex = _textures[0];
    glBindTexture(GL_TEXTURE_2D, _tex);

    assert(tex.header.format == Texture::Format::RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.header.width, tex.header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data.data());

    tex.data.clear();
  }
}

void PositionBuffer::update()
{
  if(_vbo == 0)
  {
    glGenBuffers(1, &_vbo);
  }

  if(!_positions.empty())
  {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * _positions.size(), _positions.data(), GL_STATIC_DRAW);

    _size = _positions.size();
    _positions.clear();
  }
}

void NormalBuffer::update()
{
  
}

void UvBuffer::update()
{
  if(_vbo == 0)
  {
    glGenBuffers(1, &_vbo);
  }


  if(!_uvs.empty())
  {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Uv) * _uvs.size(), _uvs.data(), GL_STATIC_DRAW);

    _uvs.clear();
  }
}

void TransparencyBuffer::update()
{
  
}


void initialize()
{
  GLenum glewInitResult = glewInit();
  if (glewInitResult != GLEW_OK)
  {
      throw std::runtime_error("Failed to initialize GLEW");
  }
  std::cout << "[gal] initialized OpenGL with " << glGetString(GL_VERSION) << ", " << glGetString(GL_RENDERER) << std::endl;

  glDisable(GL_DEPTH_TEST);
  glClearColor(1.f, 0.f, 1.f, 1.f);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  impl::_default_viewport.x = viewport[0];
  impl::_default_viewport.y = viewport[1];
  impl::_default_viewport.width = viewport[2];
  impl::_default_viewport.height = viewport[3];

  impl::_shader = createShaderProgram();
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

  impl::Renderer::render(impl::_draw_buffer.get_front());
}


}