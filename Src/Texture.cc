/**
 *   @file: Texture.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Engine.hh"
#include "Texture.hh"

Texture::Texture() {}

optional<Texture ptr> Texture::add(string cref texture_path)
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

    Texture ptr texture = &texture_map[texture_path];
    texture->width = width;
    texture->height = height;
    texture->src_channels = src_channels;

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

optional<Texture ptr> Texture::get(string cref texture_name)
{
    Log::info("Getting texture \"{}\"...", texture_name);
    auto iter = texture_map.find(texture_name);
    if (iter == texture_map.end())
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
    return texture_map.contains(texture_name);
}