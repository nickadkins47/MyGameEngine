/**
 *   @file: Chunk.hh
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#pragma once

#include <glm/vec3.hpp>

#include "Cube.hh"
#include "../Core.hh"

class Model;
class Texture;

class VoxChunk
{
    public:
    
    VoxChunk();

    delete_other_ops(VoxChunk)

    bool is_gen = false; //is generated
    bool is_ren = false; //is rendered

    int inline static seed = 0;

    Texture inline static ptr temp_tex = nullptr;

    size_t inline static const x_dim = 8;
    size_t inline static const y_dim = 8;
    size_t inline static const z_dim = 16;

    //returns reference to the block value at (x,y,z)
    VoxCube ref at(int x, int y, int z);

    void generate(int cx, int cy);

    void register_cube(int lx, int x, int ly, int y, int z, six<bool> cref open_sides);

    void finalize();

    protected:

    Model ptr model;

    array<array<array<VoxCube, z_dim>, y_dim>, x_dim> _chunk {}; //3d array of cube IDs

    void add_quad(array<glm::vec3, 8> cref v, array<int, 4> vi, glm::vec3 normals);

    float inline static const qs = 1.0f; //qs / quadSize: physical size of a quad / side of a cube

    //Perlin noise function, given an x & y coordinate, # of octaves, persistence value, and a scalar + vertical shift value
    static int perlin(int x, int y, float in_scale, float out_shift, float out_scale);
};