/**
 *   @file: Mesh.hh
 * @author: Nicholas Adkins
 *   @date: Sep 16 2025
 *  @brief: 
 */

#include "Core.hh"
#include "Shader.hh"

#pragma once

class Texture;

union Vertex
{
    int static const size = 8;
    float at[size] = {};
    struct
    {
        float x, y, z; //position
        float nx,ny,nz; //normals
        float tx,ty; //texture coords
    };
};

class Mesh
{
    public:

    Mesh();

    //deleteOtherOps(Mesh)

    GLuint VAO, VBO, EBO;
    
    //bool use_EBO; //TODO

    //given vertices, generate optimal vertices & indices vectors for this mesh
    //void gen_optimal_v_i(vector<float> cref base_verts);

    //TODO Desc: Creates the model's VAO, VBO, and EBO, from the vertices & indices
    void gen_gl_data();

    //Renders the model on screen, based on the model matrix
    //that is loaded into the shader beforehand
    //(Also binds the model's VAO, VBO, and EBO)
    void render(Shader cref shader) const;

    //protected:

    vector<Vertex> vertices;
    vector<uint> indices;
    vector<Texture ptr> textures;

    static void vertex_attribute_array(GLenum val_type, vector<uint> cref attributes);

    static constexpr int sizeof_gl_type(GLenum type);

};