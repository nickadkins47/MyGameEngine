/**
 *   @file: Texture.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include "Globals.hh"

class Texture
{
    public:

    Texture(string const& tex_path, GLenum img_format);

    deleteOtherOps(Texture)

    GLuint id;
    int width = 0, height = 0, nr_channels = 0;
};