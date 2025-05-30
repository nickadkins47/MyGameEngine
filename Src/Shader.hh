/**
 *   @file: Shader.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Globals.hh"

class Shader
{
    public:
    
    //Reads & Builds the Shader
    Shader(string const& vert_path, string const& frag_path);
    
    deleteOtherOps(Shader)

    //Shader program ID
    GLuint id;

    //Use/Activate the Shader; AKA glUseProgram(id);
    void use() const;

    //Set uniform bool value ${name} to ${value}
    void set_bool(string const& name, bool value) const;

    //Set uniform int value ${name} to ${value}
    void set_int(string const& name, int value) const;

    //Set uniform float value ${name} to ${value}
    void set_float(string const& name, float value) const;

    protected:

    //...

    private:

    //...
};