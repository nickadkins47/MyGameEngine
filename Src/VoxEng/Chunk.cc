/**
 *   @file: Chunk.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include "../Ext/GL/Enum.hh"
#include "../Ext/GL/Functions.hh"
#include <glm/vec3.hpp>
#include <stb/stb_perlin.h>

#include "Chunk.hh"

VoxChunk::VoxChunk() {}

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
    is_generated = true;
}

void VoxChunk::register_cube(int x, int y, int z, six<bool> cref open_sides)
{
    int const gx = qs * x; //graphical/simulation location x
    int const gy = qs * y; //etc
    int const gz = qs * z; //etc

    for (int i = 0; i < open_sides.size(); i++)
    {
        if (open_sides[i])
            mesh.append_range(array{gx, gy, gz, i});
    }
}

void VoxChunk::load()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(int), mesh.data(), GL_STATIC_DRAW);

    glVertexAttribIPointer(0, 3, GL_INT, 4*sizeof(int), r_cast<void ptr>(0*sizeof(int)));
    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(1, 1, GL_INT, 4*sizeof(int), r_cast<void ptr>(3*sizeof(int)));
    glEnableVertexAttribArray(1);

    is_loaded = true;
}

void VoxChunk::render()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, cast<int>(mesh.size()));
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
