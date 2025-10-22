/**
 *   @file: Shader.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Manager.hh"

class Texture;

class Shader : public Manager<Shader>
{
    public:

    //TODO DESC: Reads & Builds the Shader based on the provided vert & frag paths
    static bool add(string cref shader_name,
        string_view vert_path, string_view geom_path, string_view frag_path,
        int num_lights = 0, int num_textures = 0);
    
    delete_other_ops(Shader)

    uint ID = 0; //Shader program ID
    int num_textures = 0;

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

    Shader(); //do not manually call. always use _::add() instead

    protected:

    //TEMP DESC: add either a vert/geom/frag shader (mode = 0/1/2 respectively)
    static optional<uint> add_one_shader(string_view shader_name, string_view sh_path, int mode);
};