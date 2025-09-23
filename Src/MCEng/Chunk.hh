/**
 *   @file: Chunk.hh
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#pragma once

#include "CubeIDs.hh"
#include "../Core.hh"

class Model;

class MyChunk
{
    public:
    
    MyChunk();

    deleteOtherOps(MyChunk)

    //returns reference to the block ID at (x,y,z)
    CubeID ref at(int x, int y, int z);

    //generate this chunk's block IDs
    void generate(int cx, int cy);

    //sends quad data for a given cube to this->quadData
    void register_cube(int lx, int x, int ly, int y, int z, six<bool> cref open_sides);

    //gives this->vertices to the model
    //void make_model();

    bool is_gen = false; //is generated
    bool is_ren = false; //is rendered

    int inline static seed = 0;

    size_t inline static const x_dim = 8;
    size_t inline static const y_dim = 8;
    size_t inline static const z_dim = 16;

    array<Model ptr, 6> inline static quad_models;

    protected:

    array<array<array<CubeID, z_dim>, y_dim>, x_dim> _chunk {}; //3d array of cube IDs

    //vector<float> vertices;

    //given v & 4 indices for v, extract each of the 4 vec3s, concatenate them,
    //return it, plus two texture coordinates per vertex
    /* static constexpr array<float, 20> get_from_v_at_indices(
        array<glm::vec3, 8> cref v, int i1, int i2, int i3, int i4
    ); */

    //Perlin noise function, given an x & y coordinate, # of octaves, persistence value, and a scalar + vertical shift value
    static int perlin(int x, int y, float in_scale, float out_shift, float out_scale);

    float inline static const qs = 1.0f; //qs / quadSize: physical size of a quad / side of a cube
};