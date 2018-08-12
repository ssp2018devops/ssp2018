#include <gal/gal.hpp>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <cassert>
#include <sstream>

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



struct Mesh
{
  PositionBuffer* positions = nullptr;
  NormalBuffer* normals = nullptr;
  UvBuffer* uvs = nullptr;
  TransparencyBuffer* transparencies = nullptr;
  GLuint vao = 0;
  bool is_dirty = true;
};

struct PositionBuffer
{
  std::vector<Position> positions;
  size_t size = 0;
  GLuint vbo = 0;
};

struct NormalBuffer
{
  std::vector<Normal> normals;
  GLuint vbo = 0;
};

struct UvBuffer
{
  std::vector<Uv> uvs;
  GLuint vbo = 0;
};

struct TransparencyBuffer
{
  std::vector<Transparency> transparencies;
};

struct Draw
{
  TransformBuffer* transforms = nullptr;
  TextureBuffer* textures = nullptr;
  Mesh* mesh = nullptr;
  Viewport viewport;
};

struct TransformBuffer
{
  std::vector<Transform> transforms;
};

struct InternalTexture
{
  Texture header;
  std::vector<char> data;
};

struct TextureBuffer
{
  std::vector<InternalTexture> textures;
  GLuint tex = 0;
};

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

  _default_viewport.x = viewport[0];
  _default_viewport.y = viewport[1];
  _default_viewport.width = viewport[2];
  _default_viewport.height = viewport[3];

  _shader = createShaderProgram();
}

Draw* create_draw()
{
  Draw* draw = new Draw();
  draw->viewport = _default_viewport;
  return draw;
}

Mesh* create_mesh()
{
  return new Mesh();
}


PositionBuffer* create_position_buffer()
{
  return new PositionBuffer();
}

NormalBuffer* create_normal_buffer()
{
  return new NormalBuffer();
}

UvBuffer* create_uv_buffer()
{
  return new UvBuffer();
}

TransparencyBuffer* create_transparency_buffer()
{
  return new TransparencyBuffer();
}

TransformBuffer* create_transform_buffer()
{
  return new TransformBuffer();
}

TextureBuffer* create_texture_buffer()
{
  return new TextureBuffer();
}


void destroy(Draw* draw)
{
  delete draw;
}

void destroy(Mesh* mesh)
{
  delete mesh;
}

void destroy(PositionBuffer* buffer)
{
  delete buffer;
}

void destroy(NormalBuffer* buffer)
{
  delete buffer;
}

void destroy(UvBuffer* buffer)
{
  delete buffer;
}

void destroy(TransparencyBuffer* buffer)
{
  delete buffer;
}

void destroy(TransformBuffer* buffer)
{
  delete buffer;
}

void destroy(TextureBuffer* buffer)
{
  delete buffer;
}


void set(Draw* draw, Mesh* mesh)
{
  draw->mesh = mesh;
}

void set(Draw* draw, TransformBuffer* buffer)
{
  draw->transforms = buffer;
}

void set(Draw* draw, TextureBuffer* buffer)
{
  draw->textures = buffer;
}

void set(Draw* draw, const Viewport& viewport)
{
  draw->viewport = viewport;
}

void set(Mesh* mesh, PositionBuffer* buffer)
{
  mesh->positions = buffer;
  mesh->is_dirty = true;
}

void set(Mesh* mesh, NormalBuffer* buffer)
{
  mesh->normals = buffer;
  mesh->is_dirty = true;
}

void set(Mesh* mesh, UvBuffer* buffer)
{
  mesh->uvs = buffer;
  mesh->is_dirty = true;
}

void set(Mesh* mesh, TransparencyBuffer* buffer)
{
  mesh->transparencies = buffer;
  mesh->is_dirty = true;
}

void set(PositionBuffer* buffer, const Position* data, size_t count)
{
  buffer->positions.assign(data, data + count);
}

void set(NormalBuffer* buffer, const Normal* data, size_t count)
{
  buffer->normals.assign(data, data + count);
}

void set(UvBuffer* buffer, const Uv* data, size_t count)
{
  buffer->uvs.assign(data, data + count);
}

void set(TransparencyBuffer* buffer, const Transparency* data, size_t count)
{
  buffer->transparencies.assign(data, data + count);
}

void set(TransformBuffer* buffer, const Transform* data, size_t count)
{
  buffer->transforms.assign(data, data + count);
}

void set(TextureBuffer* buffer, const Texture* data, size_t count)
{
  buffer->textures.resize(count);
  for(size_t i = 0; i < count; i++)
  {
    InternalTexture& tex = buffer->textures[i];
    tex.header = data[i];
    assert(tex.header.format == Texture::Format::RGBA);
    tex.data.assign(tex.header.data, tex.header.data + tex.header.width * tex.header.height * 4);
  }
}


Position* stream(PositionBuffer* buffer, size_t count)
{
  return nullptr;
}

Normal* stream(NormalBuffer* buffer, size_t count)
{
  return nullptr;
}

Uv* stream(UvBuffer* buffer, size_t count)
{
  return nullptr;
}

Transparency* stream(TransparencyBuffer* buffer, size_t count)
{
  return nullptr;
}

Transform* stream(TransformBuffer* buffer, size_t count)
{
  return nullptr;
}

Texture* stream(TextureBuffer* buffer, size_t count)
{
  return nullptr;
}


void draw(Draw* draw)
{
  _draw_buffer.get_back().push_back(draw);
}

void render()
{
  _draw_buffer.swap();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(Draw* draw : _draw_buffer.get_front())
  {
    if(draw->mesh->vao == 0)
    {
      glGenVertexArrays(1, &draw->mesh->vao);
      glBindVertexArray(draw->mesh->vao);

      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
    }

    if(draw->mesh->positions->vbo == 0)
    {
      glGenBuffers(1, &draw->mesh->positions->vbo);
    }

    if(draw->mesh->uvs->vbo == 0)
    {
      glGenBuffers(1, &draw->mesh->uvs->vbo);
    }

    if(!draw->mesh->positions->positions.empty())
    {
      PositionBuffer& buf = *draw->mesh->positions;
      glBindBuffer(GL_ARRAY_BUFFER, buf.vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * buf.positions.size(), buf.positions.data(), GL_STATIC_DRAW);

      buf.size = buf.positions.size();
      buf.positions.clear();
    }

    if(!draw->mesh->uvs->uvs.empty())
    {
      UvBuffer& buf = *draw->mesh->uvs;
      glBindBuffer(GL_ARRAY_BUFFER, buf.vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Uv) * buf.uvs.size(), buf.uvs.data(), GL_STATIC_DRAW);

      buf.uvs.clear();
    }

    if(draw->mesh->is_dirty)
    {
      Mesh& mesh = *draw->mesh;
      mesh.is_dirty = false;
	    glBindVertexArray(mesh.vao);

      glBindBuffer(GL_ARRAY_BUFFER, mesh.positions->vbo);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Position), 0);

      glBindBuffer(GL_ARRAY_BUFFER, mesh.uvs->vbo);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Uv), 0);
    }


    if(draw->textures->tex == 0)
    {
      glGenTextures(1, &draw->textures->tex);
      glBindTexture(GL_TEXTURE_2D, draw->textures->tex);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }

    if(!draw->textures->textures[0].data.empty())
    {
      InternalTexture& tex = draw->textures->textures[0];
      glBindTexture(GL_TEXTURE_2D, draw->textures->tex);

      assert(tex.header.format == Texture::Format::RGBA);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.header.width, tex.header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data.data());

      tex.data.clear();
    }
  
    glBindVertexArray(draw->mesh->vao);
    glBindTexture(GL_TEXTURE_2D, draw->textures->tex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, draw->mesh->positions->size);
  }
}


}