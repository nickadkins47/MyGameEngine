/**
 *   @file: Grid.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include "Grid.hh"

MyGrid::MyGrid() {}

constexpr MyChunk ref MyGrid::chunk(int cx, int cy)
{
    return _grid
        [(cx < 0)? (sz_x + cx): cx]
        [(cy < 0)? (sz_y + cy): cy]; 
        //negative values will loop back around
}

CubeID ref MyGrid::at(int x, int y, int z)
{
    return this->chunk(
        cast<int>(std::floor(cast<double>(x) / MyChunk::x_dim)), //floor to lowest multiple of x/y_dim
        cast<int>(std::floor(cast<double>(y) / MyChunk::y_dim))  //
    ).at(
        x % MyChunk::x_dim, y % MyChunk::y_dim, z
    );
}

void MyGrid::load(int cx, int cy)
{
    MyChunk ref chk = this->chunk(cx,cy);
    if (chk.is_ren) return;

    for (int xs : {-1,0,1}) { //generate all neighboring chunks
        for (int ys : {-1,0,1}) {
            MyChunk ref neighbor = this->chunk(cx+xs,cy+ys);
            if (!neighbor.is_gen) neighbor.generate(cx+xs,cy+ys);
        }
    }

    for (size_t lx = 0; lx < chk.x_dim; lx++) { //local x
        int const x = lx + (cx * chk.x_dim); //global x
        for (size_t ly = 0; ly < chk.y_dim; ly++) { //local y
            int const y = ly + (cy * chk.y_dim); //global y
            for (size_t z = 0; z < chk.z_dim; z++) { //local & global z
                //if (x,y,z) is 0 (air) or undefined, dont draw any quads for that block
                if (is_open(x,y,z)) continue;
                six<bool> const openSides = {
                    is_open(x-1,y,z), //is block at(x,y,z) air/empty/invalid?
                    is_open(x+1,y,z),
                    is_open(x,y-1,z),
                    is_open(x,y+1,z),
                    is_open(x,y,z-1),
                    is_open(x,y,z+1),
                };
                chk.register_cube(x, y, z, cube_txts[chk.at(lx,ly,z)], openSides);
            }
        }
    }
    chk.make_model(cx,cy);
}

bool MyGrid::is_open(int x, int y, int z)
{
    if (z < 0 || z >= MyChunk::z_dim) return false;
    return !cube_txts.contains(this->at(x,y,z));
}