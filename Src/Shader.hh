/**
 *   @file: Shader.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

class Texture;

class Shader
{
    public:

    //TODO DESC: Reads & Builds the Shader based on the provided path
    //given (path) has no extensions, it will look for (path).vert & (path).frag
    static optional<Shader ptr> add(string cref shader_path, int num_lights = 0, int num_textures = 0);

    static optional<Shader ptr> get(string cref shader_name);

    static bool exists(string cref shader_name);
    
    delete_other_ops(Shader)

    uint ID; //Shader program ID

    //Use/Activate the Shader; AKA glUseProgram(ID);
    void use() const;

    void sampler2d(int tex_unit, Texture cptr texture) const;

    void uniform_f(string_view name, float value) const;

    void uniform_i(string_view name, int value) const;

    void uniform_u(string_view name, uint value) const;

    void uniform_fv(string_view name, int size, float cptr value) const;

    void uniform_iv(string_view name, int size, int cptr value) const;

    void uniform_uv(string_view name, int size, uint cptr value) const;

    void uniform_fm(string_view name, int cols, int rows, float cptr value, bool transpose = false) const;

    //protected:

    Shader();

    unordered_map<string, Shader> inline static shader_map;

};