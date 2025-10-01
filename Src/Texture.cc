/**
 *   @file: Texture.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Texture.hh"

Texture::Texture() {}

optional<Texture ptr> Texture::add(string cref texture_path, int type)
{
    Log::info("Adding texture \"{}\"...", texture_path);

    stbi_set_flip_vertically_on_load(true);

    int width, height, src_channels;
    //desired_channels = 4 -> will always return whatever data as RGBA
    unsigned char* data = stbi_load(texture_path.data(), &width, &height, &src_channels, 4);
    if (!data) {
        Log::warn("Adding texture \"{}\": Failed (Cannot Load)", texture_path); //TODO more detail?
        return nullopt;
    }

    Texture ptr texture = manager.get_new(texture_path);
    texture->width = width;
    texture->height = height;
    texture->src_channels = src_channels;
    texture->type = type;

    glGenTextures(1, &texture->ID);
    glBindTexture(GL_TEXTURE_2D, texture->ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //old: GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //old: GL_LINEAR

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    Log::info("Adding texture \"{}\": Success", texture_path);
    return texture;
}

manager_funcs_cc(Texture, texture_name)
