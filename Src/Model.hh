/**
 *   @file: Model.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

#include "Globals.hh"
#include "Shader.hh"
#include "Texture.hh"

class Model
{
    public:

    Model();
    Model(vector<pair<int, GLenum>> const ref attributes, vector<float> const ref vertices, vector<unsigned int> const ref indices);

    ~Model();

    GLuint VAO, VBO, EBO;
    
    //bool use_EBO; //TODO

    Shader ptr shader = nullptr;

    //Create the model's VAO, VBO, and EBO, from
    //the vertices, indices, and attributes;
    void process();

    //Binds the model's VAO, VBO, and EBO
    void bind() const;

    //Renders the model on screen, based on the model matrix
    //that is loaded into the shader beforehand
    void render() const;

    //protected:

    vector<pair<int, GLenum>> attributes;
    vector<float> vertices;
    vector<unsigned int> indices;

    vector<Texture> textures;

    static constexpr int sizeof_gl_type(GLenum type);

};