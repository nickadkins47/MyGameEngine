/**
 *   @file: Mesh.cc
 * @author: Nicholas Adkins
 *   @date: Sep 16 2025
 *  @brief: 
 */

#include "Ext/GL/Functions.hh"

#include "Mesh.hh"
#include "Model.hh"
#include "Obj.hh"
#include "Shader.hh"
#include "Texture.hh"

Mesh::Mesh(Model ptr parent)
: parent(parent)
{}

void Mesh::gen_gl_data(bool is_indexed, bool is_instanced)
{
    //TODO add option for VoxEng to use ints instead of floats for values
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    {
        glGenBuffers(1, &VBO);
        set_buffer(VBO, GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
        set_vertex_attribs(GL_FLOAT, 0, {3, 3, 2}); //positions, normals, & texcoords
    }

    if (is_instanced)
    {
        glGenBuffers(1, &IVBO);
        set_buffer(IVBO, GL_ARRAY_BUFFER, instance_m_mats, GL_DYNAMIC_DRAW);
        set_vertex_attribs(GL_FLOAT, 3, {4, 4, 4, 4}, 1); //model matrices
    }

    if (is_indexed)
    {
        glGenBuffers(1, &EBO);
        set_buffer(EBO, GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);
    }
}

void Mesh::set_textures(Shader cptr shader) const
{
    //shader should be already binded at this point

    int const tex_num = cast<int>(textures.size());
    int i = 0;
    for (/**/; i < tex_num; i++) //set all of mesh's textures
    {
        shader->sampler2d(i, textures[i]);
        shader->uniform_i(format("textures[{}].type", i), textures[i]->type);
    }
    for (/**/; i < prev_tex_num; i++) //unset any remaining texture slots
    {
        shader->uniform_i(format("textures[{}].type", i), 0);
    }
    prev_tex_num = tex_num;

    shader->uniform_f("shininess", shininess);
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);

    //TODO: more options for how to render things? IE like GL_TRIANGLE_STRIPS etc
    if (EBO == 0) //not indexed
    {
        if (IVBO == 0) //not instanced
            glDrawArrays         (GL_TRIANGLES, 0, cast<int>(vertices.size()));
        else //is instanced
            glDrawArraysInstanced(GL_TRIANGLES, 0, cast<int>(vertices.size()), cast<int>(instance_m_mats.size()));
    }
    else //is indexed
    {
        if (IVBO == 0) //not instanced
            glDrawElements         (GL_TRIANGLES, cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
        else //is instanced
            glDrawElementsInstanced(GL_TRIANGLES, cast<int>(indices.size()), GL_UNSIGNED_INT, 0, cast<int>(instance_m_mats.size()));
    }
}

void Mesh::update_instance_m_mats()
{
    if (IVBO == 0) return;

    instance_m_mats.resize(parent->parent_objs.size());
    for (int i = 0; i < cast<int>(parent->parent_objs.size()); i++)
        instance_m_mats[i] = parent->parent_objs[i]->model_mat;

    set_buffer(IVBO, GL_ARRAY_BUFFER, instance_m_mats, GL_DYNAMIC_DRAW);
}

void Mesh::set_vertex_attribs(GLenum val_type, int location, vector<uint> cref attributes, int divisor)
{
    int total_size = 0;
    for (uint attr : attributes)
        total_size += attr;
    total_size *= sizeof_gl_type(val_type);

    size_t size_thus_far = 0; //represents an address, must be size_t
    for (uint i = 0; i < attributes.size(); i++)
    {
        uint attr = attributes[i];
        int loc = i + location;
        glVertexAttribPointer(loc, attr, val_type, GL_FALSE, total_size, r_cast<void ptr>(size_thus_far));
        glVertexAttribDivisor(loc, divisor);
        glEnableVertexAttribArray(loc);
        size_thus_far += cast<size_t>(attr * sizeof_gl_type(val_type));
    }
}

constexpr int Mesh::sizeof_gl_type(GLenum val_type)
{
    switch (val_type)
    {
        case GL_FLOAT:        return sizeof(float);
        case GL_INT:          return sizeof(int);
        case GL_UNSIGNED_INT: return sizeof(uint);
        default: return 0;
    }
}
