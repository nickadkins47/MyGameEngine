/**
 *   @file: Obj.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Core.hh"
#include "Manager.hh"

class Model;
class Shader;

class Obj
{
    public:

    static optional<Obj ptr> add(string cref obj_name, string cref model_name, string cref shader_name);

    static optional<Obj ptr> add(string cref obj_name, Model ptr model, Shader ptr shader);

    manager_funcs_hh(Obj, obj_name)

    delete_other_ops(Obj)

    Model ptr model;
    Shader ptr shader;

    void render(glm::mat4 cref vp_mat) const;

    glm::vec3 get_position() const;
    void set_position(glm::vec3 cref position);
    void move_position(glm::vec3 cref position);

    void rotate(float deg, glm::vec3 cref axis);

    void scale(glm::vec3 cref factor);

    //protected:

    Obj();

    Manager<Obj> inline static manager;
    
    glm::mat4 model_mat {1.0f};

};