#pragma once

#include <cstring>

///
/// Graphics Abstraction Layer.
/// Provides a higher-level API
/// for 3D-rendering.
///
namespace gal
{
  class Draw;
  class Mesh;
  class TransformBuffer;
  class TextureBuffer;
  class PositionBuffer;
  class NormalBuffer;
  class UvBuffer;
  class TransparencyBuffer;

  ///
  /// 4x4 transform matrix.
  /// For transformations from
  /// local space to world space.
  /// 
  struct Transform
  {
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
  /// Initialize gal
  /// Must be called before any other gal call.
  ///
  void initialize();

  ///
  /// Create a draw object.
  /// A draw object is a collection of
  /// one mesh object, one transform 
  /// buffer object and one texture
  /// buffer object.
  /// \return A draw object.
  ///
  Draw* create_draw();

  ///
  /// Create a mesh object.
  /// A mesh object is a collection
  /// of one position buffer object,
  /// one normal buffer object,
  /// one uv buffer object and
  /// one transparency buffer object.
  /// \return A mesh object.
  ///
  Mesh* create_mesh();

  ///
  /// Create a position buffer object.
  /// A position buffer object holds
  /// an array of Position objects.
  /// \return A position buffer object.
  ///
  PositionBuffer* create_position_buffer();

  ///
  /// Create a normal buffer object.
  /// A normal buffer object holds
  /// an array of Normal objects.
  /// \return A normal buffer object.
  ///
  NormalBuffer* create_normal_buffer();

  ///
  /// Create a uv buffer object.
  /// A uv buffer object holds
  /// an array of Uv objects.
  /// \return A uv buffer object.
  ///
  UvBuffer* create_uv_buffer();
  
  ///
  /// Create a transparency buffer object.
  /// A transparency buffer object holds
  /// an array of Transparency objects.
  /// \return A transparency buffer object.
  ///
  TransparencyBuffer* create_transparency_buffer();

  ///
  /// Create a transform buffer object.
  /// A transform buffer object holds
  /// an array of Transform objects.
  /// \return A transform buffer object.
  ///
  TransformBuffer* create_transform_buffer();

  ///
  /// Create a texture buffer object.
  /// A texture buffer object holds
  /// an array of Texture objects.
  /// \return A texture buffer object.
  ///
  TextureBuffer* create_texture_buffer();

  ///
  /// Destroy draw object.
  /// Does not destroy objects
  /// assigned to this object.
  /// \param draw Draw object to destroy.
  ///
  void destroy(Draw* draw);

  ///
  /// Destroy mesh object.
  /// Does not destroy objects
  /// assigned to this object.
  /// \param mesh Mesh object to destroy.
  ///
  void destroy(Mesh* mesh);

  ///
  /// Destroy position buffer object.
  /// Unassigns the buffer object
  /// from all mesh objects and
  /// deletes all Position data 
  /// assigned to this buffer object.
  /// \param buffer Buffer object to destroy.
  ///
  void destroy(PositionBuffer* buffer);

  ///
  /// Destroy normal buffer object.
  /// Unassigns the buffer object
  /// from all mesh objects and
  /// deletes all Normal data 
  /// assigned to this buffer object.
  /// \param buffer Buffer buffer object to destroy.
  ///
  void destroy(NormalBuffer* buffer);

  ///
  /// Destroy uv buffer object.
  /// Unassigns the buffer object
  /// from all mesh objects and
  /// deletes all Uv data 
  /// assigned to this buffer object.
  /// \param buffer Buffer buffer object to destroy.
  ///
  void destroy(UvBuffer* buffer);

  ///
  /// Destroy transparency buffer object.
  /// Unassigns the buffer object
  /// from all mesh objects and
  /// deletes all Transparency data 
  /// assigned to this buffer object.
  /// \param buffer Buffer buffer object to destroy.
  ///
  void destroy(TransparencyBuffer* buffer);

  ///
  /// Destroy transform buffer object.
  /// Unassigns the buffer object
  /// from all draw objects and
  /// deletes all Transform data 
  /// assigned to this buffer object.
  /// \param buffer Buffer buffer object to destroy.
  ///
  void destroy(TransformBuffer* buffer);

  ///
  /// Destroy texture buffer object.
  /// Unassigns the buffer object
  /// from all draw objects and
  /// deletes all Texture  data 
  /// assigned to this buffer object.
  /// \param buffer Buffer buffer object to destroy.
  ///
  void destroy(TextureBuffer* buffer);

  ///
  /// Assign mesh object to draw object.
  /// Only one mesh object can be assigned to
  /// a draw object.
  /// \param draw Draw object to assign to.
  /// \param mesh Mesh object to assign.
  ///
  void set(Draw* draw, Mesh* mesh);

  ///
  /// Assign transform buffer object to draw object.
  /// Only one transform buffer object can be assigned to
  /// a draw object.
  /// \param draw Draw object to assign to.
  /// \param buffer Buffer object to assign.
  ///
  void set(Draw* draw, TransformBuffer* buffer);

  ///
  /// Assign texture buffer object to draw object.
  /// Only one texture buffer object can be assigned to
  /// a draw object.
  /// \param draw Draw object to assign to.
  /// \param buffer Buffer object to assign.
  ///
  void set(Draw* draw, TextureBuffer* buffer);

  ///
  /// Assign position buffer object to mesh object.
  /// Only one position buffer object can be assigned to
  /// a draw object.
  /// \param mesh Mesh object to assign to.
  /// \param buffer Buffer object to assign.
  ///
  void set(Mesh* mesh, PositionBuffer* buffer);

  ///
  /// Assign normal buffer object to mesh object.
  /// Only one normal buffer object can be assigned to
  /// a draw object.
  /// Optional.
  /// \param mesh Mesh object to assign to.
  /// \param buffer Buffer object to assign.
  ///
  void set(Mesh* mesh, NormalBuffer* buffer);

  ///
  /// Assign uv buffer object to mesh object.
  /// Only one uv buffer object can be assigned to
  /// a draw object.
  /// Optional.
  /// \param mesh Mesh object to assign to.
  /// \param buffer Buffer object to assign.
  ///
  void set(Mesh* mesh, UvBuffer* buffer);

  ///
  /// Assign transparency buffer object to mesh object.
  /// Only one transparency buffer object can be assigned to
  /// a draw object.
  /// Optional.
  /// \param mesh Mesh object to assign to.
  /// \param buffer Buffer object to assign.
  ///
  void set(Mesh* mesh, TransparencyBuffer* buffer);


  ///
  /// Assign data to buffer.
  /// Destroys previously assigned data.
  /// \param buffer Buffer object to assign to.
  /// \param data Array of data to assign.
  /// \param count Data element count.
  ///
  void set(PositionBuffer* buffer, const Position* data, size_t count);
  void set(NormalBuffer* buffer, const Normal* data, size_t count);
  void set(UvBuffer* buffer, const Uv* data, size_t count);
  void set(TransparencyBuffer* buffer, const Transparency* data, size_t count);
  void set(TransformBuffer* buffer, const Transform* data, size_t count);
  void set(TextureBuffer* buffer, const Texture* data, size_t count);

  ///
  /// Stream data to buffer.
  /// Opens a stream to the data
  /// of a buffer object.
  /// \param buffer The buffer to stream to.
  /// \param count Data element count of the stream.
  /// \return The stream.
  ///
  Position* stream(PositionBuffer* buffer, size_t count);
  Normal* stream(NormalBuffer* buffer, size_t count);
  Uv* stream(UvBuffer* buffer, size_t count);
  Transparency* stream(TransparencyBuffer* buffer, size_t count);
  Transform* stream(TransformBuffer* buffer, size_t count);
  Texture* stream(TextureBuffer* buffer, size_t count);

  ///
  /// Draw a draw object.
  /// If no mesh object is assigned,
  /// nothing is drawn.
  /// The mesh is drawn as many times
  /// as the size of the transform buffer
  /// object.
  ///
  void draw(Draw* draw);
}