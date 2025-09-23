/**
 *   @file: CubeIDs.cc
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#include "CubeIDs.hh"
#include "../Engine.hh"
#include "../Texture.hh"

void load_cube_txts()
{
    //TODO: use engine->get_texture after reworking all of this

    Texture ptr test1 = Texture::get("Textures/test/test1.png").value();
    Texture ptr test2 = Texture::get("Textures/test/test2.png").value();
    Texture ptr test3 = Texture::get("Textures/test/test3.png").value();
    Texture ptr test4 = Texture::get("Textures/test/test4.png").value();
    Texture ptr test5 = Texture::get("Textures/test/test5.png").value();
    Texture ptr test6 = Texture::get("Textures/test/test6.png").value();

    Texture ptr grass_top = Texture::get("Textures/grass_top.png").value();
    Texture ptr grass_side = Texture::get("Textures/grass_side.png").value();
    Texture ptr grass_bottom = Texture::get("Textures/grass_bottom.png").value();

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