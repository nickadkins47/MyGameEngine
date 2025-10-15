/**
 *   @file: Grid.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include "Grid.hh"

VoxGrid::VoxGrid() {}

constexpr VoxChunk ref VoxGrid::chunk(int cx, int cy)
{
    return _grid
        [(cx < 0)? (sz_x + cx): cx]
        [(cy < 0)? (sz_y + cy): cy]; 
        //negative values will loop back around
}

VoxCube ref VoxGrid::at(int x, int y, int z)
{
    return this->chunk(
        cast<int>(std::floor(cast<double>(x) / VoxChunk::x_dim)), //floor to lowest multiple of x/y_dim
        cast<int>(std::floor(cast<double>(y) / VoxChunk::y_dim))
    ).at(
        x % VoxChunk::x_dim, y % VoxChunk::y_dim, z
    );
}

void VoxGrid::load(int cx, int cy)
{
    VoxChunk ref chk = this->chunk(cx,cy);
    if (chk.is_ren) return;

    for (int xs : {-1,0,1}) //generate all neighboring chunks
    {
        for (int ys : {-1,0,1})
        {
            VoxChunk ref neighbor = this->chunk(cx+xs,cy+ys);
            if (!neighbor.is_gen) neighbor.generate(cx+xs,cy+ys);
        }
    }

    for (size_t lx = 0; lx < VoxChunk::x_dim; lx++) //local x
    {
        int const x = cast<int>(lx) + (cx * cast<int>(VoxChunk::x_dim)); //global x

        for (size_t ly = 0; ly < VoxChunk::y_dim; ly++) //local y
        {
            int const y = cast<int>(ly) + (cy * cast<int>(VoxChunk::y_dim)); //global y

            for (size_t lz = 0; lz < VoxChunk::z_dim; lz++) //local z
            {
                int const z = cast<int>(lz); //global z (== local z, for 2D chunks)

                if (is_open(x,y,z)) continue; //if (x,y,z) is 0 (air) or undefined, dont render block

                six<bool> const open_sides = {
                    is_open(x-1,y,z), //is block at(x,y,z) air/empty/invalid?
                    is_open(x+1,y,z),
                    is_open(x,y-1,z),
                    is_open(x,y+1,z),
                    is_open(x,y,z-1),
                    is_open(x,y,z+1),
                };

                chk.register_cube(cast<int>(lx), x, cast<int>(ly), y, z, open_sides);
            }
        }
    }
    chk.finalize();
}

void VoxGrid::render()
{

}

bool VoxGrid::is_open(int x, int y, int z)
{
    if (z < 0 || z >= VoxChunk::z_dim) return false;
    return !cube_txts.contains(this->at(x,y,z));
}