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
    in vec2 uvIn;
    in vec3 normalIn;
    out vec2 uv;
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
    in vec2 uv;
    in vec3 normal;

    uniform sampler2D textureSampler;

    void main()
    {
      const vec3 light_direction = normalize(vec3(1, -1, -1));
      color = texture(textureSampler, uv).rgb;

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

static_assert(std::is_same_v<GLushort, Index>, "Index must be same type as underlying graphics API");

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

void Mesh::set(const IndexBuffer& buffer)
{
  _indices = (IndexBuffer*)&buffer;
  _is_dirty = true;
}

void Mesh::set(const NormalBuffer& buffer)
{
  _normals = (NormalBuffer*)&buffer;
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
  _has_viewport = true;
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

void TransformBuffer::set(const float* data_4x4, size_t count)
{
  set((const Transform*)data_4x4, count);
}

void TransformBuffer::set(const Transform* data, size_t count)
{
  _transforms.assign(data, data + count);
  
  if(count > 0)
  {
    _is_dirty = true;
  }
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

void IndexBuffer::set(const Index* data, size_t count)
{
  _indices.assign(data, data + count);
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

IndexBuffer::IndexBuffer()
{

}

IndexBuffer::~IndexBuffer()
{
  
}

NormalBuffer::NormalBuffer()
{

}

NormalBuffer::~NormalBuffer()
{
  
}

TransformBuffer::TransformBuffer()
{

}

TransformBuffer::~TransformBuffer()
{
  
}

void IndexBuffer::update()
{
  if(_vbo == 0)
  {
    glGenBuffers(1, &_vbo);
  }

  if(!_indices.empty())
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index) * _indices.size(), _indices.data(), GL_STATIC_DRAW);
  
    _size = _indices.size();
    _indices.clear();
  }
}

void IndexBuffer::render()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);

  static_assert(std::is_same_v<Index, GLushort>, "");
  glDrawElements(GL_TRIANGLE_STRIP, _size, GL_UNSIGNED_SHORT, 0);
}

void Draw::update()
{
  if(!_has_viewport)
  {
    _viewport = impl::_default_viewport;
    _has_viewport = true;
  }

  if(_mesh)
  {
    _mesh->update();
  }

  if(_textures)
  {
    _textures->update();
  }
}

void Draw::render()
{
  _textures->bind();

  _transforms->bind(_viewport);

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
    glEnableVertexAttribArray(2);
  }

  if(_is_dirty)
  {
    _is_dirty = false;
    
    _positions->update();
    _uvs->update();

    if(_normals)
    {
      _normals->update();
    }

    if(_indices)
    {
      _indices->update();
    }
    
    glBindVertexArray(_vao);

    _positions->bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Position), 0);

    _uvs->bind();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Uv), 0);

    if(_normals)
    {
      _normals->bind();
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
    }
  }
}

void Mesh::render()
{
  glBindVertexArray(_vao);

  if(_indices)
  {
    _indices->render();
  }
  else
  {
    _positions->render();
  }
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

void TransformBuffer::bind(const Viewport& viewport)
{
  glm::mat4 proj = glm::perspective(glm::radians(45.0f), 
                                    1.0f * float(viewport.width) / float(viewport.height), 
                                    0.1f, 
                                    10.0f);


  glm::mat4 mvp = proj;
  glm::mat4 mv(1.f);
  if (!_transforms.empty())
  {
    Transform& transform = _transforms.front();
    memcpy(&mv[0][0], &transform.data[0][0], sizeof(transform.data)); 
    mvp *= mv;
  }

  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  GLint uniform_mvp = glGetUniformLocation(program, "mvp");
  GLint uniform_mv = glGetUniformLocation(program, "mv");

  glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, &mvp[0][0]);
  glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, &mv[0][0]);
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
  if(_vbo == 0)
  {
    glGenBuffers(1, &_vbo);
  }


  if(!_normals.empty())
  {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normal) * _normals.size(), _normals.data(), GL_STATIC_DRAW);

    _normals.clear();
  }
}

void NormalBuffer::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
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

  impl::Renderer::render(impl::_draw_buffer.get_front());
}


}