/**
 *   @file: Texture.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include <stb/stb_image.h>

#include "Engine.hh"
#include "Texture.hh"

Texture::Texture() {}

optional<Texture ptr> Texture::add(path cref texture_p)
{
    Log::info("Adding texture \"{}\"...", texture_p.string());

    stbi_set_flip_vertically_on_load(true);

    int width, height, num_channels;
    unsigned char* data = stbi_load(texture_p.string().c_str(), &width, &height, &num_channels, 0);
    if (!data) {
        Log::warn("Adding texture \"{}\": Failed (Cannot Load)", texture_p.string()); //TODO more detail?
        return nullopt;
    }

    GLenum img_format = 
        (num_channels == 3) ?
            GL_RGB
        : //(num_channels == 4) ?
            GL_RGBA
        ;

    Texture ptr texture = &engine->texture_map[texture_p.string()];
    texture->width = width;
    texture->height = height;
    texture->num_channels = num_channels;

    glGenTextures(1, &texture->ID);
    glBindTexture(GL_TEXTURE_2D, texture->ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //old: GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //old: GL_LINEAR

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, img_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    Log::info("Adding texture \"{}\": Success", texture_p.string());
    return texture;
}

optional<Texture ptr> Texture::get(string cref texture_name)
{
    Log::info("Getting texture \"{}\"...", texture_name);
    auto iter = engine->texture_map.find(texture_name);
    if (iter == engine->texture_map.end())
    {
        Log::warn("Getting texture \"{}\": Failed", texture_name);
        return nullopt;
    }
    else
    {
        Log::info("Getting texture \"{}\": Success", texture_name);
        return &iter->second;
    }
}

bool Texture::exists(string cref texture_name)
{
    return engine->texture_map.contains(texture_name);
}