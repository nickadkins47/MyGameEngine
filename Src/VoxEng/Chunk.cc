/**
 *   @file: Chunk.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include <glm/vec3.hpp>
#include <stb/stb_perlin.h>

#include "Chunk.hh"
#include "../Engine.hh"
#include "../Obj.hh"
#include "../Model.hh"
#include "../Texture.hh"

VoxChunk::VoxChunk()
{
    int static chunk_num = 0;
    model = Model::add(format("Chunk[{}]_model", chunk_num), {Mesh()}).value();
    chunk_num++;
}

VoxCube ref VoxChunk::at(int x, int y, int z)
{
    return _chunk[x][y][z];
}

void VoxChunk::generate(int cx, int cy)
{
    for (int lx = 0; lx < cast<int>(x_dim); lx++) //local x
    {
        int const x = lx + (cx * cast<int>(x_dim)); //global x

        for (int ly = 0; ly < cast<int>(y_dim); ly++) //local y
        {
            int const y = ly + (cy * cast<int>(y_dim)); //global y

            int const z = perlin(x, y, 0.1f, 2.0f, 4.0f); //generated z coordinate
            _chunk[lx][ly][z] = VoxCube::GRASS;
            for (int uz = z - 1; uz >= 0; uz--)
            {
                _chunk[lx][ly][uz] = VoxCube::DIRT;
            }
        }
    }
    is_gen = true;
}

void VoxChunk::register_cube(int lx, int x, int ly, int y, int z, six<bool> cref open_sides)
{
    float const gx = qs * x; //graphical/simulation location x
    float const gy = qs * y; //etc
    float const gz = qs * z; //etc
    //glm::vec3 const gpos {gx,gy,gz};

    //six<Texture ptr> ref textures = cube_txts[at(lx,ly,z)];

    array<glm::vec3, 8> const v {
        glm::vec3{   gx,    gy,    gz}, //0
        glm::vec3{   gx,    gy, gz+qs}, //1
        glm::vec3{   gx, gy+qs,    gz}, //2
        glm::vec3{   gx, gy+qs, gz+qs}, //3
        glm::vec3{gx+qs,    gy,    gz}, //4
        glm::vec3{gx+qs,    gy, gz+qs}, //5
        glm::vec3{gx+qs, gy+qs,    gz}, //6
        glm::vec3{gx+qs, gy+qs, gz+qs}, //7
    };

    if (open_sides[0]) { add_quad(v, {0,1,3,2}, glm::vec3{-1.0f,  0.0f,  0.0f}); }
    if (open_sides[1]) { add_quad(v, {6,7,5,4}, glm::vec3{ 1.0f,  0.0f,  0.0f}); }
    if (open_sides[2]) { add_quad(v, {4,5,1,0}, glm::vec3{ 0.0f, -1.0f,  0.0f}); }
    if (open_sides[3]) { add_quad(v, {2,3,7,6}, glm::vec3{ 0.0f,  1.0f,  0.0f}); }
    if (open_sides[4]) { add_quad(v, {0,2,6,4}, glm::vec3{ 0.0f,  0.0f, -1.0f}); }
    if (open_sides[5]) { add_quad(v, {5,7,3,1}, glm::vec3{ 0.0f,  0.0f,  1.0f}); }

    /* for (size_t i = 0; i < 6; i++)
    {
        static int num_quads_total = 0;
        if (!open_sides[i]) continue; //then dont make this face

        Obj ptr obj = Obj::add(format("Quad[{}]", num_quads_total), quad_models[i]).value();
        obj->move_position(gpos);
        num_quads_total++;
    } */
}

void VoxChunk::finalize()
{
    model->meshes[0].gen_gl_data();
    model->meshes[0].textures.push_back(temp_tex);

    int static chunk_num = 0;
    Obj ptr obj = Obj::add(format("Chunk[{}]_obj", chunk_num), model).value();
    chunk_num++;

    is_ren = true;
}

void VoxChunk::add_quad(array<glm::vec3, 8> cref v, array<int, 4> vi, glm::vec3 normals)
{
    Vertex vert0 {
        .x = v[vi[0]].x,
        .y = v[vi[0]].y,
        .z = v[vi[0]].z,
        .nx = normals.x,
        .ny = normals.y,
        .nz = normals.z,
        .tx = 1,
        .ty = 0,
    };
    Vertex vert1 {
        .x = v[vi[1]].x,
        .y = v[vi[1]].y,
        .z = v[vi[1]].z,
        .nx = normals.x,
        .ny = normals.y,
        .nz = normals.z,
        .tx = 1,
        .ty = 1,
    };
    Vertex vert2 {
        .x = v[vi[2]].x,
        .y = v[vi[2]].y,
        .z = v[vi[2]].z,
        .nx = normals.x,
        .ny = normals.y,
        .nz = normals.z,
        .tx = 0,
        .ty = 1,
    };
    Vertex vert3 {
        .x = v[vi[3]].x,
        .y = v[vi[3]].y,
        .z = v[vi[3]].z,
        .nx = normals.x,
        .ny = normals.y,
        .nz = normals.z,
        .tx = 0,
        .ty = 0,
    };
    model->meshes[0].vertices.append_range(vector{vert0, vert1, vert2, vert2, vert3, vert0});
}

int VoxChunk::perlin(int x, int y, float in_scale, float out_shift, float out_scale)
{
    float res = stb_perlin_noise3_seed(
        cast<float>(x) * in_scale + 0.5f,
        cast<float>(y) * in_scale + 0.5f,
        0.5f,
        0, 0, 0,
        seed
    );
    res = out_scale * (res + out_shift);
    return cast<int>(res);
}
