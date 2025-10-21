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

class VoxChunk
{
    public:
    
    VoxChunk();

    delete_other_ops(VoxChunk)

    bool is_generated = false; //is the chunk's cube data created
    bool is_loaded = false; //is the chunk's mesh data created

    int inline static seed = 0;

    size_t inline static const x_dim = 8;
    size_t inline static const y_dim = 8;
    size_t inline static const z_dim = 16;

    int inline static const qs = 1; //qs / quadSize: physical size of a quad / side of a cube

    //returns reference to the block value at (x,y,z)
    VoxCube ref at(int x, int y, int z);

    void generate(int cx, int cy);
    
    void register_cube(int x, int y, int z, six<bool> cref faces);

    void load();

    void render();

    protected:

    uint VAO = 0;  //Vertex Array Object
    uint VBO = 0;  //Vertex Buffer Object

    vector<int> mesh;

    array<array<array<VoxCube, z_dim>, y_dim>, x_dim> _chunk {}; //3d array of cube IDs

    //Perlin noise function, given an x & y coordinate, # of octaves, persistence value, and a scalar + vertical shift value
    static int perlin(int x, int y, float in_scale, float out_shift, float out_scale);
};