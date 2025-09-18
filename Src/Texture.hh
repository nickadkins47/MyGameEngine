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

    Texture(string cref tex_path);

    deleteOtherOps(Texture)

    GLuint ID;
    int type = 0; //type recieved from assimp
    int width = 0, height = 0, num_channels = 0;

};