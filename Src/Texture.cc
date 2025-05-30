/**
 *   @file: Texture.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include "Texture.hh"

Texture::Texture(string const& tex_path, GLenum img_format)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(get_file(tex_path).c_str(), &width, &height, &nr_channels, 0);
    if (!data) {
        print("ERROR: Texture failed to load\n"); //TODO?
        return;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, img_format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}