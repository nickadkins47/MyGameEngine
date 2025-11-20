/**
 *   @file: Grid.hh
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#pragma once

#include "Chunk.hh"
#include "Cube.hh"
#include "../Core.hh"

class Shader;
class Texture;

class VoxGrid
{
    public:

    struct DrawComData {
        uint vert_count;
        uint instance_count;
        uint vert_first;
        int base_instance;
    };

    VoxGrid();

    delete_other_ops(VoxGrid)
    
    Texture inline static ptr temp_tex = nullptr;
    Shader inline static ptr shader = nullptr;

    size_t inline static const sz_x = 4; //num of chunks wide the world is; x direction
    size_t inline static const sz_y = 4; //num of chunks wide the world is; y direction

    //return chunk based on (cx,cy) coordinates
    VoxChunk ptr chunk(int cx, int cy);

    //return Cube ID at (x,y,z)
    VoxCube ref at(int x, int y, int z);

    //TODO DESC: calls a function for each chunk in the grid
    void for_each_chunk(function<void(int, int)> cref func);

    //load chunk; also generates Cube IDs for neighboring chunks
    void load(int cx, int cy);

    //create unified VAO out of chunk data
    void finalize();

    //TODO DESC: renders chunks to screen
    void render();

    protected:

    uint VAO = 0; //Vertex Array Object
    uint VBO = 0; //Vertex Buffer Object
    uint IBO = 0; //Indirect Buffer Object

    vector<int> vert_data;
    vector<DrawComData> draw_data;

    array<array<VoxChunk ptr, sz_x>, sz_y> _grid; //Grid of Chunks, each of which is 32x32x32 blocks

    bool is_open(int x, int y, int z); //true if block at (x,y,z) is air/empty/invalid
};