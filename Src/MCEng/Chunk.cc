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
#include "../Shader.hh"
#include "../Texture.hh"

MyChunk::MyChunk() {}

CubeID ref MyChunk::at(int x, int y, int z)
{
    return _chunk[x][y][z];
}

void MyChunk::generate(int cx, int cy)
{
    for (int lx = 0; lx < cast<int>(x_dim); lx++) //local x
    {
        int const x = lx + (cx * cast<int>(x_dim)); //global x

        for (int ly = 0; ly < cast<int>(y_dim); ly++) //local y
        {
            int const y = ly + (cy * cast<int>(y_dim)); //global y

            int const z = perlin(x, y, 0.1f, 2.0f, 4.0f); //generated z coordinate
            _chunk[lx][ly][z] = CubeID::GRASS;
            for (int uz = z - 1; uz >= 0; uz--)
            {
                _chunk[lx][ly][uz] = CubeID::DIRT;
            }
        }
    }
    is_gen = true;
}

void MyChunk::register_cube(int lx, int x, int ly, int y, int z, six<bool> cref open_sides)
{
    float const gx = qs * x; //graphical/simulation location x
    float const gy = qs * y; //etc
    float const gz = qs * z; //etc
    glm::vec3 const gpos {gx,gy,gz};

    //six<Texture ptr> ref textures = cube_txts[at(lx,ly,z)];

    for (size_t i = 0; i < 6; i++)
    {
        static int num_quads_total = 0;
        if (!open_sides[i]) continue; //then dont make this face

        Obj ptr obj = Obj::add(format("Quad[{}]", num_quads_total), quad_models[i], default_sh).value();
        obj->move_position(gpos);
    }
}

int MyChunk::perlin(int x, int y, float in_scale, float out_shift, float out_scale)
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
