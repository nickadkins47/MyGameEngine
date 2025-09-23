/**
 *   @file: Mesh.cc
 * @author: Nicholas Adkins
 *   @date: Sep 16 2025
 *  @brief: 
 */

#include "Mesh.hh"
#include "Texture.hh"

Mesh::Mesh() {}

void Mesh::gen_gl_data()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

    //TEMP: right now solely allows for just position/normals/texcoords 3/3/2
    //TODO: flexible vertex attribute arrays
    vertex_attribute_array(GL_FLOAT, {3, 3, 2});
}

void Mesh::render(Shader cptr shader) const
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    for (int i = 0; i < textures.size(); i++)
    {
        shader->sampler2d(i, *textures[i]);
        shader->uniform_i(format("textures[{}].type", i), textures[i]->type);
    }
    shader->uniform_f("shininess", shininess);

    //TODO: more options for how to render things? IE like GL_QUADS
    glDrawElements(GL_TRIANGLES, cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

void Mesh::vertex_attribute_array(GLenum val_type, vector<uint> cref attributes)
{
    int total_size = 0;
    for (uint cref attr : attributes)
        total_size += (attr * sizeof_gl_type(val_type));

    int64_t size_thus_far = 0; //TODO: maybe issue on 32bit OSes?
    for (uint i = 0; i < attributes.size(); i++)
    {
        uint cref attr = attributes[i];
        glVertexAttribPointer(i, attr, val_type, GL_FALSE, total_size, r_cast<void ptr>(size_thus_far)); 
        glEnableVertexAttribArray(i);
        size_thus_far += (attr * sizeof_gl_type(val_type));
    }
}

constexpr int Mesh::sizeof_gl_type(GLenum val_type)
{
    switch (val_type)
    {
        case GL_FLOAT:        return sizeof(float);
        case GL_INT:          return sizeof(int);
        case GL_UNSIGNED_INT: return sizeof(uint);
        default: return -1;
    }
}
