/**
 *   @file: Obj.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Model.hh"

class Obj
{
    public:

    Obj(Model ptr model = nullptr, Shader ptr shader = nullptr);

    Model ptr model;
    Shader ptr shader;

    void render(glm::mat4 cref vp_mat) const;

    glm::vec3 get_position() const;
    void set_position(glm::vec3 cref position);
    void move_position(glm::vec3 cref position);

    void rotate(float deg, glm::vec3 cref axis);

    void scale(glm::vec3 cref factor);

    //protected:
    
    glm::mat4 model_mat {1.0f};

    Texture ptr diffuse = nullptr;
    Texture ptr specular = nullptr;
    float shininess = 0;
    vector<Texture ptr> textures;

};