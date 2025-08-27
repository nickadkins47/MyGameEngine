/**
 *   @file: Chunk.hh
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#pragma once

#include <stb/stb_perlin.h>

#include "CubeIDs.hh"
#include "../Core.hh"
#include "../Model.hh"
#include "../Obj.hh"
#include "../Texture.hh"

class MyGrid;

class MyChunk : public Obj
{
    public:
    
    MyChunk();

    deleteOtherOps(MyChunk)

    //returns reference to the block ID at (x,y,z)
    CubeID ref at(int x, int y, int z);

    //generate this chunk's block IDs
    void generate(int cx, int cy);

    //sends quad data for a given cube to this->quadData
    void register_cube(
        int x, int y, int z, six<Texture> cref textures, six<bool> cref open_sides
    );

    //gives this->vertices to the model
    void make_model();

    bool is_gen = false; //is generated
    bool is_ren = false; //is rendered

    int inline static seed = 0;

    Shader inline static ptr shader = nullptr;

    size_t inline static const x_dim = 8;
    size_t inline static const y_dim = 8;
    size_t inline static const z_dim = 16;

    private:

    array<array<array<CubeID, z_dim>, y_dim>, x_dim> _chunk {}; //3d array of cube IDs

    vector<float> vertices;

    //given v & 4 indices for v, extract each of the 4 vec3s, concatenate them,
    //return it, plus two texture coordinates per vertex
    static constexpr array<float, 20> get_from_v_at_indices(
        array<glm::vec3, 8> cref v, int i1, int i2, int i3, int i4
    );

    //Perlin noise function, given an x & y coordinate, # of octaves, persistence value, and a scalar + vertical shift value
    static double perlin2d(double x, double y, double scale, double shift, int32_t octaves, double persistence = 0.5);

    float inline static const qs = 5.0f; //qs / quadSize: physical size of a quad / side of a cube
};