#include <gal/gal.hpp>

namespace gal
{

void initialize()
{

}

Draw* create_draw()
{
  return nullptr;
}

Mesh* create_mesh()
{
  return nullptr;
}


PositionBuffer* create_position_buffer()
{
  return nullptr;
}

NormalBuffer* create_normal_buffer()
{
  return nullptr;
}

UvBuffer* create_uv_buffer()
{
  return nullptr;
}

TransparencyBuffer* create_transparency_buffer()
{
  return nullptr;
}

TransformBuffer* create_transform_buffer()
{
  return nullptr;
}

TextureBuffer* create_texture_buffer()
{
  return nullptr;
}


void destroy(Draw* draw)
{

}

void destroy(Mesh* mesh)
{

}

void destroy(PositionBuffer* buffer)
{

}

void destroy(NormalBuffer* buffer)
{

}

void destroy(UvBuffer* buffer)
{

}

void destroy(TransparencyBuffer* buffer)
{

}

void destroy(TransformBuffer* buffer)
{

}

void destroy(TextureBuffer* buffer)
{

}


void set(Draw* draw, Mesh* mesh)
{

}

void set(Draw* draw, TransformBuffer* buffer)
{

}

void set(Draw* draw, TextureBuffer* buffer)
{

}


void set(Mesh* mesh, PositionBuffer* buffer)
{

}

void set(Mesh* mesh, NormalBuffer* buffer)
{

}

void set(Mesh* mesh, UvBuffer* buffer)
{

}

void set(Mesh* mesh, TransparencyBuffer* buffer)
{

}



void set(PositionBuffer* buffer, const Position* data, size_t count)
{

}

void set(NormalBuffer* buffer, const Normal* data, size_t count)
{

}

void set(UvBuffer* buffer, const Uv* data, size_t count)
{

}

void set(TransparencyBuffer* buffer, const Transparency* data, size_t count)
{

}

void set(TransformBuffer* buffer, const Transform* data, size_t count)
{

}

void set(TextureBuffer* buffer, const Texture* data, size_t count)
{

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

}



}