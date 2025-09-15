/**
 *   @file: Model.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Shader.hh"
#include "Texture.hh"

class Model
{
    public:

    Model();
    Model(GLenum val_type, vector<GLenum> cref attributes, vector<float> cref vertices, vector<uint> cref indices);

    ~Model();

    GLuint VAO, VBO, EBO;
    
    //bool use_EBO; //TODO

    //given vertices, generate optimal vertices & indices vectors for this model
    void gen_optimal_v_i(vector<float> cref base_verts);

    //Create the model's VAO, VBO, and EBO, from
    //the vertices, indices, and attributes;
    void process();

    //Renders the model on screen, based on the model matrix
    //that is loaded into the shader beforehand
    //(Also binds the model's VAO, VBO, and EBO)
    void render() const;

    //protected:

    GLenum val_type;
    vector<GLenum> attributes;
    vector<float> vertices; //TODO: template model class to allow for ints alongside floats
    vector<uint> indices;

    static constexpr int sizeof_gl_type(GLenum type);

};