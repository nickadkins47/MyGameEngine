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

    static optional<Texture ptr> add(path cref texture_p);

    static optional<Texture ptr> get(string cref texture_name);

    static bool exists(string cref texture_name);

    deleteOtherOps(Texture)

    GLuint ID;
    int type = 0; //type recieved from assimp
    int width = 0, height = 0, num_channels = 0;

    //protected:

    Texture();

};