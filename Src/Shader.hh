/**
 *   @file: Shader.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Texture.hh"

//Sets a shader uniform value of a given name,
//based on the given value and its type
#define set_uniform(type, shader, name, value) \
    glUniform##type##(glGetUniformLocation(shader.ID, name), value);

//Sets a shader uniform vector of a given name,
//based on the given value and its type
#define set_uniform_vec(type, shader, name, value) \
    glUniform##type##v##(glGetUniformLocation(shader.ID, name), 1, glm::value_ptr(value));

//Sets a shader uniform matrix of a given name,
//based on the given value and its type
#define set_uniform_mat(type, shader, name, value) \
    glUniformMatrix##type##v##(glGetUniformLocation(shader.ID, name), 1, GL_FALSE, glm::value_ptr(value));

class Shader
{
    public:
    
    //Reads & Builds the Shader
    Shader(string const ref vert_path, string const ref frag_path, int tex_units);
    
    deleteOtherOps(Shader)

    //Shader program ID
    GLuint ID;

    //Use/Activate the Shader; AKA glUseProgram(ID);
    void use() const;

    //Sets Shader's Sampler2D tex #_ to the ID of the given Texture
    void set_texture(int tex_unit, Texture const ref texture) const;

    protected:

    //...

    private:

    //...
};