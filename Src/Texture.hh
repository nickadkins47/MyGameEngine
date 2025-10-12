/**
 *   @file: Texture.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Manager.hh"

class Texture : public Manager<Texture>
{
    public:

    static optional<Texture ptr> add(string cref texture_path, int type);

    delete_other_ops(Texture)

    uint ID = 0;
    int type = 0; //refer to assimp texture types
    int width = 0, height = 0, src_channels = 0; //data recieved from stbi_load

    Texture(); //do not manually call. always use _::add() instead
};