
#include <gal/gal.hpp>

#include <vector>

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
    return 0;
}

