/**
 *   @file: Texture.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include "Ext/GL/Enum.hh"
#include "Ext/GL/Functions.hh"
#include <stb/stb_image.h>

#include "Texture.hh"

Texture::Texture() {}

Texture::~Texture()
{
    stbi_image_free(data);
}

optional<Texture ptr> Texture::add(string cref texture_path, int type)
{
    Log log("Adding texture \"{}\"", texture_path);

    stbi_set_flip_vertically_on_load(true);

    Texture ptr texture = new_val(texture_path);
    texture->type = type;

    //desired_channels = 4 -> will always return whatever data as RGBA
    texture->data = stbi_load(texture_path.data(), &texture->width, &texture->height, &texture->src_channels, 4);
    if (!texture->data) {
        log.fail("Cannot Load"); //TODO more detail?
        Texture::remove(texture_path);
        return nullopt;
    }

    glGenTextures(1, &texture->ID);
    glBindTexture(GL_TEXTURE_2D, texture->ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //old: GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //old: GL_LINEAR

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}
