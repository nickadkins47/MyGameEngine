/**
 *   @file: Shader.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Texture.hh"

//inline glm::vec3 inside glm::value_ptr
#define ivec3(...) glm::value_ptr(glm::vec3(__VA_ARGS__))

class Shader
{
    public:
    
    //Reads & Builds the Shader
    Shader(string cref vert_path, string cref frag_path, int tex_units);
    
    deleteOtherOps(Shader)

    //Shader program ID
    GLuint ID;

    //Use/Activate the Shader; AKA glUseProgram(ID);
    void use() const;

    //Sets Shader's Sampler2D tex #_ to the ID of the given Texture
    void set_texture(int tex_unit, Texture cref texture) const;

    void uniform_f(string cref name, float value) const;

    void uniform_i(string cref name, int value) const;

    void uniform_u(string cref name, uint value) const;

    void uniform_fv(string cref name, int size, float cptr value) const;

    void uniform_iv(string cref name, int size, int cptr value) const;

    void uniform_uv(string cref name, int size, uint cptr value) const;

    void uniform_fm(string cref name, int cols, int rows, float cptr value, bool transpose = false) const;

    protected:

    //...

};