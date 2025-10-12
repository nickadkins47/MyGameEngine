/**
 *   @file: Mesh.hh
 * @author: Nicholas Adkins
 *   @date: Sep 16 2025
 *  @brief: 
 */

#pragma once

#include "Ext/GL/Enum.hh"
#include "Ext/GL/Functions.hh"
#include <glm/mat4x4.hpp>

#include "Core.hh"

class Obj;
class Model;
class Shader;
class Texture;

union Vertex
{
    int static const size = 8;
    float at[size] = {};
    struct
    {
        float x, y, z;  //position
        float nx,ny,nz; //normals
        float tx,ty;    //texture coords
    };
};

class Mesh
{
    public:

    Mesh(Model ptr parent);

    uint VAO = 0;  //Vertex Array Object
    uint VBO = 0;  //Vertex Buffer Object (Main)
    uint IVBO = 0; //Vertex Buffer Object (Instances)
    uint EBO = 0;  //Element Buffer Object

    vector<Vertex> vertices;
    vector<Texture ptr> textures;
    float shininess = 32.0f; //TEMP default value

    vector<uint> indices;

    vector<glm::mat4> instance_m_mats;

    Model ptr parent;

    //TODO Desc: Creates the model's VAO, VBO, and EBO, from the vertices & indices
    void gen_gl_data(bool is_indexed = false, bool is_instanced = false);

    void set_textures(Shader cptr shader) const;

    void draw() const;

    //TODO DESC: updates instance mat4 values. Expensive, so use sparingly
    void update_instance_m_mats();

    static void set_vertex_attribs(GLenum val_type, int location, vector<uint> cref attributes, int divisor = 0);

    static constexpr int sizeof_gl_type(GLenum type);

    template<typename T>
    inline static void set_buffer(uint ID, GLenum type, vector<T> cref data, GLenum usage)
    {
        glBindBuffer(type, ID);
        glBufferData(type, data.size() * sizeof(T), data.data(), usage);
    }

};