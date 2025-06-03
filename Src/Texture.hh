/**
 *   @file: Texture.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Globals.hh"

class Texture
{
    public:

    Texture(string const ref tex_path, GLenum img_format);

    //deleteOtherOps(Texture)

    GLuint ID;
    int width = 0, height = 0, nr_channels = 0;
};