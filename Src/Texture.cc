/**
 *   @file: Texture.cc
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#include <stb/stb_image.h>

#include "Texture.hh"

Texture::Texture(string cref tex_path)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(get_file_path(tex_path).c_str(), &width, &height, &num_channels, 0);
    if (!data) {
        print("ERROR: Texture failed to load ({})\n", tex_path); //TODO?
        return;
    }

    //print("File(\"{}\", width:{}, height:{}, channels:{})\n", tex_path, width, height, num_channels);

    GLenum img_format = 
        (num_channels == 3) ?
            GL_RGB
        : //(num_channels == 4) ?
            GL_RGBA
        ;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //old: GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //old: GL_LINEAR

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, img_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //print("{}, {}\n", tex_path, ID);

    stbi_image_free(data);
}