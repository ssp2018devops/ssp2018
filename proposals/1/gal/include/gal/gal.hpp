#pragma once

#include <cstring>
#include <vector>


///
/// Graphics Abstraction Layer.
/// Provides a higher-level API
/// for 3D-rendering.
///
namespace gal
{
  ///
  /// 4x4 transform matrix.
  /// For transformations from
  /// local space to view space.
  /// 
  struct Transform
  {
    Transform(const float* matrix_4x4)
    {
      memcpy(data, matrix_4x4, sizeof(data));
    }
    
    Transform& operator=(const float* matrix_4x4)
    {
      memcpy(data, matrix_4x4, sizeof(data));
      return *this;
    }


    float data[4][4];
  };

  ///
  /// Vertex position.
  ///
  struct Position
  {
    float x;
    float y;
    float z;
  };

  ///
  /// Vertex normal.
  ///
  struct Normal
  {
    float x;
    float y;
    float z;
  };

  ///
  /// Vertex uv.
  ///
  struct Uv
  {
    float u;
    float v;
    float w;
  };

  ///
  /// Vertex in a mesh
  ///
  struct Vertex
  {
    Position position;
    Normal normal;
    Uv uv;
  };

  ///
  /// Viewport.
  ///
  struct Viewport
  {
    int x;
    int y;
    size_t width;
    size_t height;
  };

  ///
  /// Texture data.
  ///
  struct TextureData
  {
    enum class Format : unsigned char
    {
      RGB,
      RGBA,
      // ...
    };

    const char* data;
    size_t width;
    size_t height;
    Format format;
  };

  ///
  /// Index
  ///
  using Index = unsigned short;

  ///
  /// Example mesh data structure
  /// Holds data in an optimized way
  /// to reduce loading times. This
  /// struct may change depending
  /// on how we want our meshes to
  /// be stored.
  ///
  struct MeshData
  {
     std::vector<Vertex> vertices;
     std::vector<Index> indices;
  };

  ///
  /// Render draw calls to screen.
  ///
  void render();

  namespace impl
  {
    class Mesh;
    class Texture;
    class Renderer;
  }

  ///
  /// Mesh resource.
  ///
  class Mesh
  {
    public:
      ///
      /// Create mesh from data.
      ///
      Mesh(const MeshData& data);

      ///
      /// Destroy mesh.
      ///
      ~Mesh();

      Mesh(const Mesh& other) = delete;
      Mesh& operator=(const Mesh& other) = delete;

    private:
      friend class Draw;

      impl::Mesh* _mesh;
  };


  ///
  /// Texture resource.
  ///
  class Texture
  {
    public:
      ///
      /// Create texture from data.
      ///
      Texture(const TextureData& data);
      
      ///
      /// Destroy texture.
      //
      ~Texture();

      Texture(const Texture& other) = delete;
      Texture& operator=(const Texture& other) = delete;

    private:
      friend class Draw;

      impl::Texture* _texture;
  };

  ///
  /// Draw instance of a mesh.
  /// 
  class Draw
  {
    public:
      ///
      /// Set mesh.
      /// \param mesh Mesh to assign.
      ///
      void set(const Mesh& mesh);

      ///
      /// Set transforms.
      /// \param transforms Transforms to assign.
      ///
      void set(const std::vector<Transform>& transforms);

      ///
      /// Set textures.
      /// \param textures Textures to assign.
      ///
      void set(const std::vector<Texture*>& textures);

      ///
      /// Set viewport.
      /// \param viewport Viewport to assign.
      ///
      void set(const Viewport& viewport);

      ///
      /// Draw the mesh.
      /// If no mesh object is assigned,
      /// nothing is drawn.
      /// The mesh is drawn as many times
      /// as the number of assigned
      /// transforms.
      ///
      void draw() const;

    private:
      friend class impl::Renderer;
      void initialize();
      void render();


      impl::Mesh* _mesh = nullptr;
      std::vector<impl::Texture*> _textures;
      std::vector<Transform> _transforms;
      Viewport _viewport;
      bool _has_viewport = false;
      
  };
}