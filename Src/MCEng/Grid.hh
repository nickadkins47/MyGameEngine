/**
 *   @file: Grid.hh
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#pragma once

#include "Chunk.hh"
#include "CubeIDs.hh"
#include "../Core.hh"

class MyGrid
{
    public:

    MyGrid();
    
    deleteOtherOps(MyGrid)

    //return chunk based on (cx,cy) coordinates
    constexpr MyChunk ref chunk(int cx, int cy);

    //return Cube ID at (x,y,z)
    CubeID ref at(int x, int y, int z);

    //load/render chunk; also generates Cube IDs for neighboring chunks
    void load(int cx, int cy);

    inline static const size_t sz_x = 32; // 32x32 chunks in the world
    inline static const size_t sz_y = 32; // 32x32 chunks in the world

    private:

    bool is_open(int x, int y, int z); //true if block at (x,y,z) is air/empty/invalid

    array<array<MyChunk, sz_x>, sz_y> _grid; //Grid of Chunks, each of which is a 16x16x32 grid
};