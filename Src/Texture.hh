/**
 *   @file: Texture.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Manager.hh"

class Texture
{
    public:

    static optional<Texture ptr> add(string cref texture_path, int type);

    manager_funcs_hh(Texture, texture_name)

    delete_other_ops(Texture)

    uint ID;
    int type = 0; //type recieved from assimp
    int width = 0, height = 0, src_channels = 0; //data recieved from stbi_load

    Texture();

    Manager<Texture> inline static manager;

};