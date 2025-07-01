/**
 *   @file: Model.cc
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#include "Model.hh"

Model::Model() {}

Model::Model(vector<pair<int, GLenum>> cref attributes, vector<float> cref vertices, vector<uint> cref indices)
: attributes(attributes), vertices(vertices), indices(indices) {}

Model::~Model()
{
    delete(shader);
}

void Model::gen_optimal_v_i(vector<float> cref base_verts)
{
    //TEMP: just make some indices
    //TODO: move to chunk & redo it since texture coords will mess it up
    /* vertices = base_verts;
    uint num_quads = cast<uint>(base_verts.size() / 4);
    for (uint i = 0; i < num_quads; i += 4)
    {
        indices.append_range(vector<uint>{
            i+0, i+1, i+2,  i+2, i+3, i+0,
        });
    } */
}

void Model::process()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

    int total_size = 0;
    for (auto cref [num, type] : attributes)
        total_size += (num * sizeof_gl_type(type));
        //TODO: Compare this with std::ranges solution? after getting FPS counter to work

    int64_t size_thus_far = 0; //TODO: maybe issue on 32bit OSes?
    //for (size_t i = 0; i < attributes.size(); i++)
    for (auto [i, attr] : std::views::enumerate(attributes))
    {
        auto [num, type] = attr;
        glVertexAttribPointer(i, num, type, GL_FALSE, total_size, r_cast<void ptr>(size_thus_far)); 
        glEnableVertexAttribArray(i);
        size_thus_far += (num * sizeof_gl_type(type));
    }
}

void Model::bind() const
{
    shader->use();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    for (auto [i, tex] : std::views::enumerate(textures))
        shader->set_texture(i, tex);
}

void Model::render() const
{
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

constexpr int Model::sizeof_gl_type(GLenum type)
{
    switch (type)
    {
        case GL_FLOAT:        return sizeof(float);
        case GL_INT:          return sizeof(int);
        case GL_UNSIGNED_INT: return sizeof(uint);
        default: return -1;
    }
}