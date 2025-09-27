/**
 *   @file: Shader.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Texture.hh"

class Shader
{
    public:

    //TODO DESC: Reads & Builds the Shader based on the provided path
    //given (path) has no extensions, it will look for (path).vert & (path).frag
    static optional<Shader ptr> add(path cref shader_p, int num_lights = 0, int num_textures = 0);

    static optional<Shader ptr> get(string cref shader_name);

    static bool exists(string cref shader_name);
    
    deleteOtherOps(Shader)

    GLuint ID; //Shader program ID

    //Use/Activate the Shader; AKA glUseProgram(ID);
    void use() const;

    void sampler2d(int tex_unit, Texture cref texture) const;

    void uniform_f(string cref name, float value) const;

    void uniform_i(string cref name, int value) const;

    void uniform_u(string cref name, uint value) const;

    void uniform_fv(string cref name, int size, float cptr value) const;

    void uniform_iv(string cref name, int size, int cptr value) const;

    void uniform_uv(string cref name, int size, uint cptr value) const;

    void uniform_fm(string cref name, int cols, int rows, float cptr value, bool transpose = false) const;

    //protected:

    Shader();

};