/**
 *   @file: Texture.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

class Texture
{
    public:

    static optional<Texture ptr> add(string cref texture_path);

    static optional<Texture ptr> get(string cref texture_name);

    static bool exists(string cref texture_name);

    delete_other_ops(Texture)

    uint ID;
    int type = 0; //type recieved from assimp
    int width = 0, height = 0, src_channels = 0; //data recieved from stbi_load

    //protected:

    Texture();

    unordered_map<string, Texture> inline static texture_map;

};