#pragma once

#include <cstring>

namespace gal
{
  class Draw;

  class Transforms;
  class Textures;

  struct Transform
  {
    // ...
  };

  class Mesh;

  class Positions;
  class Normals;
  class Uvs;
  class Transparencies;


  struct Position
  {
    float x;
    float y;
    float z;
  };

  struct Normal
  {
    float x;
    float y;
    float z;
  };

  struct Uv
  {
    float u;
    float v;
  };

  struct Transparency
  {
    float transparency;
  };

  struct Texture
  {
    const char* data;
    size_t width;
    size_t height;
    int format;
  };


  Mesh* create_mesh();
  void delete_mesh(Mesh* mesh);

  Positions* create_positions();
  Normals* create_normals();
  // ...

  void set(Mesh* mesh, Positions* positions);
  void set(Mesh* mesh, Normals* normals);
  // ...

  void set(Positions* positions, const Position* data, size_t count);
  void set(Normals* normals, const Normal* data, size_t count);
  // ..

  Position* stream(Positions* positions, size_t count);
  Normal* stream(Normals* normals, size_t count);
  // ..


  Textures* create_textures();
  void delete_texture(Textures* textures);
  void set(Textures* textures, const Texture* data, size_t count);


  Draw* create_draw();
  void delete_draw(Draw* draw);

  Transforms* create_transforms();
  void delete_transforms(Transforms* transforms);

  void set(Transforms* transforms, const Transform* data, size_t count);

  void set(Draw* draw, Textures* texture);
  void set(Draw* draw, Mesh* mesh);
  void set(Draw* draw, Transforms* transforms);


  void draw(Draw* draw);





  /////////////////////////////////////////////////////

  Mesh* create_mesh();
  void delete_mesh(Mesh* mesh);

  Texture* create_texture();
  void delete_texture(Texture* texture);

  using Float3 = float[3];
  using Float2 = float[2];
  using Float1 = float;
  struct VertexPnut
  {
    Float3 position;
    Float3 normal;
    Float2 uv;
    Float1 transparency;
  };





  //void set_positions(Mesh* mesh, const Float3* positions, size_t count);
  //void set_normals(Mesh* mesh, const Float3* normals, size_t count);
  //void set_uvs(Mesh* mesh, const Float2* uvs, size_t count);
  //void set_transparencies(Mesh* mesh, const Float1* transparencies, size_t count);
  void set_vertices(Mesh* mesh, const VertexPnut* vertices, size_t count);
  void set_texture(Mesh* mesh, Texture* texture);

  //void set_transparent(Mesh* mesh, bool value);

  //Float3* stream_positions(Mesh* mesh, size_t count);
  //Float3* stream_normals(Mesh* mesh, size_t count);
  //Float2* stream_uvs(Mesh* mesh, size_t count);
  VertexPnut* stream_vertices_pnut(Mesh* mesh, size_t count);

  using Hook = void (*)(const char* function, int code, const char* message);
  void hook_errors(Hook hook);
  void hook_warnings(Hook hook);


}
