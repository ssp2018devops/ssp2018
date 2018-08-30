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
  /// local space to world space.
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
  /// Vertex transparency.
  ///
  struct Transparency
  {
    float transparency;
  };

  ///
  /// Texture data.
  ///
  struct Texture
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
  /// Index
  ///
  using Index = unsigned short;


  namespace impl
  {
    using Id = unsigned int;
    class Renderer;
  }

  class Draw;
  class Mesh;
  class TransformBuffer;
  class TextureBuffer;
  class TransparencyBuffer;
  class PositionBuffer;
  class NormalBuffer;
  class UvBuffer;
  class IndexBuffer;

  class Draw
  {
    public:
      ///
      /// Indicates how to draw the mesh.
      /// 
      enum class Type : unsigned char
      {
        basic,
        point_light,
        directional_light,
      };

      ///
      /// Create a draw object.
      /// A draw object is a collection of
      /// one mesh object, one transform 
      /// buffer object and one texture
      /// buffer object.
      ///
      Draw();

      ///
      /// Assign mesh object to draw object.
      /// Only one mesh object can be assigned to
      /// a draw object.
      /// \param mesh Mesh object to assign.
      ///
      void set(const Mesh& mesh);

      ///
      /// Assign transform buffer object to draw object.
      /// Only one transform or position buffer object 
      /// can be assigned to a draw object at the same
      /// time.
      /// \param buffer Buffer object to assign.
      ///
      void set(const TransformBuffer& buffer);

      ///
      /// Assign position buffer object to draw object.
      /// Only one transform or position buffer object 
      /// can be assigned to a draw object at the same
      /// time.
      /// \param buffer Buffer object to assign.
      ///
      void set(const PositionBuffer& buffer);

      ///
      /// Assign texture buffer object to draw object.
      /// Only one texture buffer object can be assigned to
      /// a draw object.
      /// \param buffer Buffer object to assign.
      ///
      void set(const TextureBuffer& buffer);

      ///
      /// Assign viewport to draw object.
      /// Only one viewport can be assigned to
      /// a draw object.
      /// \param viewport Viewport to assign.
      ///
      void set(const Viewport& viewport);

      ///
      /// Set draw type.
      /// Default is "basic".
      /// \param type Type to set.
      ///
      void set(Type type);

      ///
      /// Draw a draw object.
      /// If no mesh object is assigned,
      /// nothing is drawn.
      /// The mesh is drawn as many times
      /// as the size of the transform buffer
      /// object.
      ///
      void draw() const;

    private:
      friend class impl::Renderer;
      void update();
      void render();

      TransformBuffer* _transforms = nullptr;
      PositionBuffer* _positions = nullptr;
      TextureBuffer* _textures = nullptr;
      Mesh* _mesh = nullptr;
      Viewport _viewport;
      bool _has_viewport = false;
      Type _type = Type::basic;
  };

  class Mesh
  {
    public:
      ///
      /// Create a mesh object.
      /// A mesh object is a collection
      /// of one position buffer object,
      /// one normal buffer object,
      /// one uv buffer object and
      /// one transparency buffer object.
      ///
      Mesh();

      ///
      /// Destroy mesh object.
      /// Does not destroy objects
      /// assigned to this object.
      ///
      ~Mesh();

      ///
      /// Assign position buffer object to mesh object.
      /// Only one position buffer object can be assigned to
      /// a mesh object.
      /// \param buffer Buffer object to assign.
      ///
      void set(const PositionBuffer& buffer);

      ///
      /// Assign normal buffer object to mesh object.
      /// Only one normal buffer object can be assigned to
      /// a mesh object.
      /// Optional.
      /// \param buffer Buffer object to assign.
      ///
      void set(const NormalBuffer& buffer);

      ///
      /// Assign uv buffer object to mesh object.
      /// Only one uv buffer object can be assigned to
      /// a mesh object.
      /// Optional.
      /// \param buffer Buffer object to assign.
      ///
      void set(const UvBuffer& buffer);

      ///
      /// Assign transparency buffer object to mesh object.
      /// Only one transparency buffer object can be assigned to
      /// a mesh object.
      /// Optional.
      /// \param buffer Buffer object to assign.
      ///
      void set(const TransparencyBuffer& buffer);

      ///
      /// Assign index buffer object to mesh object.
      /// Only one index buffer object can be assigned to
      /// a mesh object.
      /// Optional.
      /// \param buffer Buffer object to assign.
      ///
      void set(const IndexBuffer& buffer);



    private:
      friend class Draw;
      void update();
      void render();

      PositionBuffer* _positions = nullptr;
      NormalBuffer* _normals = nullptr;
      UvBuffer* _uvs = nullptr;
      TransparencyBuffer* _transparencies = nullptr;
      IndexBuffer* _indices = nullptr;
      impl::Id _vao = 0;
      bool _is_dirty = true;
  };

  class TransformBuffer
  {
    public:
      ///
      /// Create a transform buffer object.
      /// A transform buffer object holds
      /// an array of Transform objects.
      ///
      TransformBuffer();

      ///
      /// Destroy transform buffer object.
      /// Unassigns the buffer object
      /// from all draw objects and
      /// deletes all Transform data 
      /// assigned to this buffer object.
      ///
      ~TransformBuffer();

      TransformBuffer(const TransformBuffer& other) = delete;
      TransformBuffer& operator=(const TransformBuffer& other) = delete;

      ///
      /// Assign data to buffer.
      /// Closes all open streams to the buffer object.
      /// \param data Array of data to assign.
      /// \param count Data element count.
      ///
      void set(const Transform* data, size_t count);

      void set(const float* data_4x4, size_t count);

      ///
      /// Stream data to buffer.
      /// Opens an uninitialized stream to the data
      /// of a buffer object. Closes all open streams
      /// to the buffer object.
      /// \param count Data element count of the stream.
      /// \return The stream.
      ///
      Transform* stream(size_t count);

    private:
      friend class Draw;
      void bind(const Viewport& viewport);

      std::vector<Transform> _transforms;
      bool _is_dirty = false;
  };

  class TextureBuffer
  {
    public:
      ///
      /// Create a texture buffer object.
      /// A texture buffer object holds
      /// an array of Texture objects.
      ///
      TextureBuffer();

      ///
      /// Destroy texture buffer object.
      /// Unassigns the buffer object
      /// from all draw objects and
      /// deletes all Texture  data 
      /// assigned to this buffer object.
      ///
      ~TextureBuffer();

      TextureBuffer(const TextureBuffer& other) = delete;
      TextureBuffer& operator=(const TextureBuffer& other) = delete;

      ///
      /// Assign data to buffer.
      /// Closes all open streams to the buffer object.
      /// \param data Array of data to assign.
      /// \param count Data element count.
      ///
      void set(const Texture* data, size_t count);

      ///
      /// Stream data to buffer.
      /// Opens an uninitialized stream to the data
      /// of a buffer object. Closes all open streams
      /// to the buffer object.
      /// \param count Data element count of the stream.
      /// \return The stream.
      ///
      Texture* stream(size_t count);

    private:
      friend class Draw;
      void update();
      void bind();

      struct InternalTexture
      {
        Texture header;
        std::vector<char> data;
      };

      std::vector<InternalTexture> _textures;
      impl::Id _tex = 0;
  };

  class PositionBuffer
  {
    public:
      ///
      /// Create a position buffer object.
      /// A position buffer object holds
      /// an array of Position objects.
      ///
      PositionBuffer();

      ///
      /// Destroy position buffer object.
      /// Unassigns the buffer object
      /// from all mesh objects and
      /// deletes all Position data 
      /// assigned to this buffer object.
      ///
      ~PositionBuffer();

      PositionBuffer(const PositionBuffer& other) = delete;
      PositionBuffer& operator=(const PositionBuffer& other) = delete;

      ///
      /// Assign data to buffer.
      /// Closes all open streams to the buffer object.
      /// \param data Array of data to assign.
      /// \param count Data element count.
      ///
      void set(const Position* data, size_t count);

      ///
      /// Stream data to buffer.
      /// Opens an uninitialized stream to the data
      /// of a buffer object. Closes all open streams
      /// to the buffer object.
      /// \param count Data element count of the stream.
      /// \return The stream.
      ///
      Position* stream(size_t count);

    private:
      friend class Mesh;
      void update();
      void bind();
      void render();

      std::vector<Position> _positions;
      size_t _size = 0;
      impl::Id _vbo = 0;
  };

  class NormalBuffer
  {
    public:
      ///
      /// Create a normal buffer object.
      /// A normal buffer object holds
      /// an array of Normal objects.
      ///
      NormalBuffer();


      ///
      /// Destroy normal buffer object.
      /// Unassigns the buffer object
      /// from all mesh objects and
      /// deletes all Normal data 
      /// assigned to this buffer object.
      ///
      ~NormalBuffer();

      NormalBuffer(const NormalBuffer& other) = delete;
      NormalBuffer& operator=(const NormalBuffer& other) = delete;

      ///
      /// Assign data to buffer.
      /// Closes all open streams to the buffer object.
      /// \param data Array of data to assign.
      /// \param count Data element count.
      ///
      void set(const Normal* data, size_t count);

      ///
      /// Stream data to buffer.
      /// Opens an uninitialized stream to the data
      /// of a buffer object. Closes all open streams
      /// to the buffer object.
      /// \param count Data element count of the stream.
      /// \return The stream.
      ///
      Normal* stream(size_t count);

    private:
      friend class Mesh;
      void update();
      void bind();

      std::vector<Normal> _normals;
      impl::Id _vbo = 0;
  };

  class UvBuffer
  {
    public:
      ///
      /// Create a uv buffer object.
      /// A uv buffer object holds
      /// an array of Uv objects.
      ///
      UvBuffer();

      ///
      /// Destroy uv buffer object.
      /// Unassigns the buffer object
      /// from all mesh objects and
      /// deletes all Uv data 
      /// assigned to this buffer object.
      ///
      ~UvBuffer();

      UvBuffer(const UvBuffer& other) = delete;
      UvBuffer& operator=(const UvBuffer& other) = delete;

      ///
      /// Assign data to buffer.
      /// Closes all open streams to the buffer object.
      /// \param data Array of data to assign.
      /// \param count Data element count.
      ///
      void set(const Uv* data, size_t count);

      ///
      /// Stream data to buffer.
      /// Opens an uninitialized stream to the data
      /// of a buffer object. Closes all open streams
      /// to the buffer object.
      /// \param count Data element count of the stream.
      /// \return The stream.
      ///
      Uv* stream(size_t count);

    private:
      friend class Mesh;
      void update();
      void bind();

      std::vector<Uv> _uvs;
      impl::Id _vbo = 0;
  };

  class TransparencyBuffer
  {
    public:
      ///
      /// Create a transparency buffer object.
      /// A transparency buffer object holds
      /// an array of Transparency objects.
      ///
      TransparencyBuffer();

      ///
      /// Destroy transparency buffer object.
      /// Unassigns the buffer object
      /// from all mesh objects and
      /// deletes all Transparency data 
      /// assigned to this buffer object.
      ///
      ~TransparencyBuffer();

      TransparencyBuffer(const TransparencyBuffer& other) = delete;
      TransparencyBuffer& operator=(const TransparencyBuffer& other) = delete;

      ///
      /// Assign data to buffer.
      /// Closes all open streams to the buffer object.
      /// \param data Array of data to assign.
      /// \param count Data element count.
      ///
      void set(const Transparency* data, size_t count);

      ///
      /// Stream data to buffer.
      /// Opens an uninitialized stream to the data
      /// of a buffer object. Closes all open streams
      /// to the buffer object.
      /// \param count Data element count of the stream.
      /// \return The stream.
      ///
      Transparency* stream(size_t count);

    private:
      void update();

      std::vector<Transparency> _transparencies;
  };

  class IndexBuffer
  {
    public:
      ///
      /// Create a transparency buffer object.
      /// A transparency buffer object holds
      /// an array of Transparency objects.
      ///
      IndexBuffer();

      ///
      /// Destroy transparency buffer object.
      /// Unassigns the buffer object
      /// from all mesh objects and
      /// deletes all Transparency data 
      /// assigned to this buffer object.
      ///
      ~IndexBuffer();

      IndexBuffer(const IndexBuffer& other) = delete;
      IndexBuffer& operator=(const IndexBuffer& other) = delete;

      ///
      /// Assign data to buffer.
      /// Closes all open streams to the buffer object.
      /// \param data Array of data to assign.
      /// \param count Data element count.
      ///
      void set(const Index* data, size_t count);

      ///
      /// Stream data to buffer.
      /// Opens an uninitialized stream to the data
      /// of a buffer object. Closes all open streams
      /// to the buffer object.
      /// \param count Data element count of the stream.
      /// \return The stream.
      ///
      Index* stream(size_t count);

    private:
      friend class Mesh;
      void update();
      void render();

      std::vector<Index> _indices;
      size_t _size = 0;
      impl::Id _vbo = 0;
  };

  ///
  /// Render draw calls to screen.
  ///
  void render();
}