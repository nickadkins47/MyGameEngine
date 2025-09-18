/**
 *   @file: CubeIDs.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include "CubeIDs.hh"
#include "../Engine.hh"

void load_cube_txts()
{
    //TODO: use engine->get_texture after reworking all of this

    Texture ptr test1 = &engine->get_texture("Textures/test/test1.png");
    Texture ptr test2 = &engine->get_texture("Textures/test/test2.png");
    Texture ptr test3 = &engine->get_texture("Textures/test/test3.png");
    Texture ptr test4 = &engine->get_texture("Textures/test/test4.png");
    Texture ptr test5 = &engine->get_texture("Textures/test/test5.png");
    Texture ptr test6 = &engine->get_texture("Textures/test/test6.png");

    Texture ptr grass_top = &engine->get_texture("Textures/grass_top.png");
    Texture ptr grass_side = &engine->get_texture("Textures/grass_side.png");
    Texture ptr grass_bottom = &engine->get_texture("Textures/grass_bottom.png");

    cube_txts = {
        //0 or Undefined value -> "Air" / Empty
        { CubeID::TEST, six<Texture ptr>{ //test block
            test1,
            test2,
            test3,
            test4,
            test5,
            test6
        }},
        { CubeID::GRASS, six<Texture ptr>{ //grass
            grass_side,
            grass_side,
            grass_side,
            grass_side,
            grass_bottom,
            grass_top
        }},
        { CubeID::DIRT, six<Texture ptr>{ //dirt
            grass_bottom,
            grass_bottom,
            grass_bottom,
            grass_bottom,
            grass_bottom,
            grass_bottom
        }},
    };
}