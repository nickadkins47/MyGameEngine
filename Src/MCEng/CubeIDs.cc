/**
 *   @file: CubeIDs.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include "CubeIDs.hh"

void load_cube_txts()
{
    Texture test1("Textures/test/test1.png", GL_RGBA);
    Texture test2("Textures/test/test2.png", GL_RGBA);
    Texture test3("Textures/test/test3.png", GL_RGBA);
    Texture test4("Textures/test/test4.png", GL_RGBA);
    Texture test5("Textures/test/test5.png", GL_RGBA);
    Texture test6("Textures/test/test6.png", GL_RGBA);
    
    Texture grass_top("Textures/grass_top.png", GL_RGBA);
    Texture grass_side("Textures/grass_side.png", GL_RGBA);
    Texture grass_bottom("Textures/grass_bottom.png", GL_RGBA);

    cube_txts = {
        //0 or Undefined value -> "Air" / Empty
        { CubeID::TEST, six<Texture>{ //test block
            test1,
            test2,
            test3,
            test4,
            test5,
            test6
        }},
        { CubeID::GRASS, six<Texture>{ //grass
            grass_side,
            grass_side,
            grass_side,
            grass_side,
            grass_bottom,
            grass_top
        }},
        { CubeID::DIRT, six<Texture>{ //dirt
            grass_bottom,
            grass_bottom,
            grass_bottom,
            grass_bottom,
            grass_bottom,
            grass_bottom
        }},
    };
}