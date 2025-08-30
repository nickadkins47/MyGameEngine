/**
 *   @file: Model.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

#include <utility>
    using std::pair;

#include "Core.hh"
#include "Shader.hh"
#include "Texture.hh"

class Obj;

class Model
{
    public:

    Model();
    Model(vector<pair<int, GLenum>> cref attributes, vector<float> cref vertices, vector<uint> cref indices);

    ~Model();

    GLuint VAO, VBO, EBO;
    
    //bool use_EBO; //TODO

    Shader ptr shader = nullptr;

    //given vertices, generate optimal vertices & indices vectors for this model
    void gen_optimal_v_i(vector<float> cref base_verts);

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
    vector<uint> indices;

    vector<Texture> textures;

    static constexpr int sizeof_gl_type(GLenum type);

};