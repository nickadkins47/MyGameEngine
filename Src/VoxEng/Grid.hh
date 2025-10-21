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

    VoxGrid();
    
    delete_other_ops(VoxGrid)

    //return chunk based on (cx,cy) coordinates
    VoxChunk ptr chunk(int cx, int cy);

    //return Cube ID at (x,y,z)
    VoxCube ref at(int x, int y, int z);

    //load chunk; also generates Cube IDs for neighboring chunks
    void load(int cx, int cy);

    //TODO DESC: renders chunks to screen
    void render();

    Texture inline static ptr temp_tex = nullptr;
    Shader inline static ptr shader = nullptr;

    size_t inline static const sz_x = 8; //num of chunks wide the world is; x direction
    size_t inline static const sz_y = 8; //num of chunks wide the world is; y direction

    protected:

    bool is_open(int x, int y, int z); //true if block at (x,y,z) is air/empty/invalid

    array<array<VoxChunk ptr, sz_x>, sz_y> _grid; //Grid of Chunks, each of which is a 16x16x32 grid
};