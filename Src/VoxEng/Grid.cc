/**
 *   @file: Grid.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include "../Ext/GL/Enum.hh"
#include "../Ext/GL/Functions.hh"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Grid.hh"
#include "../Mesh.hh"
#include "../Shader.hh"
#include "../Texture.hh"

VoxGrid::VoxGrid()
{
    for (auto ref row : _grid)
        for (auto ref chk : row)
            chk = new VoxChunk();
}

VoxChunk ptr VoxGrid::chunk(int cx, int cy)
{
    return _grid
        [(cx < 0)? (sz_x + cx): cx]
        [(cy < 0)? (sz_y + cy): cy] 
    ; //negative values will loop back around
}

VoxCube ref VoxGrid::at(int x, int y, int z)
{
    return this->chunk(
        cast<int>(std::floor(cast<double>(x) / VoxChunk::x_dim)), //floor to lowest multiple of x/y_dim
        cast<int>(std::floor(cast<double>(y) / VoxChunk::y_dim))
    )->at(
        x % VoxChunk::x_dim, y % VoxChunk::y_dim, z
    );
}

void VoxGrid::for_each_chunk(function<void(int, int)> cref func)
{
    int const h_sz_x = VoxGrid::sz_x / 2; //half of sz_x
    int const h_sz_y = VoxGrid::sz_y / 2; //half of sz_y
    
    for (int cx = - h_sz_x; cx < h_sz_x; cx++)
    {
        for (int cy = - h_sz_y; cy < h_sz_y; cy++)
        {
            func(cx, cy);
        }
    }
}

void VoxGrid::load(int cx, int cy)
{
    VoxChunk ptr chk = this->chunk(cx,cy);
    if (chk->is_loaded) return;

    for (int xs : {-1,0,1}) //generate all neighboring chunks
    {
        for (int ys : {-1,0,1})
        {
            VoxChunk ptr neighbor = this->chunk(cx+xs,cy+ys);
            if (!neighbor->is_generated) neighbor->generate(cx+xs,cy+ys);
        }
    }

    for (size_t lx = 0; lx < VoxChunk::x_dim; lx++) //local x
    {
        int const x = cast<int>(lx) + (cx * cast<int>(VoxChunk::x_dim)); //global x

        for (size_t ly = 0; ly < VoxChunk::y_dim; ly++) //local y
        {
            int const y = cast<int>(ly) + (cy * cast<int>(VoxChunk::y_dim)); //global y

            for (size_t lz = 0; lz < VoxChunk::z_dim; lz++) //local z
            {
                int const z = cast<int>(lz); //global z (== local z, for 2D chunks)

                if (is_open(x,y,z)) continue; //if (x,y,z) is 0 (air) or undefined, dont render block

                six<bool> const faces = { //open sides of cube(x,y,z)
                    is_open(x-1,y,z), //is block at(x,y,z) air/empty/invalid?
                    is_open(x+1,y,z),
                    is_open(x,y-1,z),
                    is_open(x,y+1,z),
                    is_open(x,y,z-1),
                    is_open(x,y,z+1),
                };

                chk->register_cube(x, y, z, faces);
            }
        }
    }

    chk->is_loaded = true;
}

void VoxGrid::finalize()
{
    for_each_chunk([this](int cx, int cy){
        int chk_val = 0
            | ((cx & 65535) << 16)
            | ((cy & 65535) << 0 )
        ;
        VoxChunk ptr chk = chunk(cx,cy);
        draw_data.emplace_back() = {
            .vert_count = cast<uint>(chk->mesh.size()),
            .instance_count = 1,
            .vert_first = cast<uint>(vert_data.size()),
            .base_instance = chk_val,
        };
        vert_data.append_range(chk->mesh);
    });

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vert_data.size() * sizeof(int), vert_data.data(), GL_STATIC_DRAW);

    glVertexAttribIPointer(0, 1, GL_INT, 1*sizeof(int), r_cast<void ptr>(0*sizeof(int)));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, IBO);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, draw_data.size() * sizeof(DrawComData), draw_data.data(), GL_DYNAMIC_DRAW);

}

void VoxGrid::render()
{
    shader->use();

    shader->sampler2d(0, temp_tex);
    shader->uniform_i("textures[0].type", temp_tex->type);

    for (int i = 1; i < Mesh::prev_tex_num; i++) //unset any remaining texture slots
    {
        shader->uniform_i(format("textures[{}].type", i), 0);
    }
    Mesh::prev_tex_num = 1;

    shader->uniform_f("shininess", 32.0f);

    glFrontFace(GL_CCW);

    glBindVertexArray(VAO);
    glMultiDrawArraysIndirect(GL_POINTS, 0, cast<int>(draw_data.size()), 0);
}

bool VoxGrid::is_open(int x, int y, int z)
{
    if (z < 0 || z >= cast<int>(VoxChunk::z_dim)) return false;
    return !cube_txts.contains(this->at(x,y,z));
}
