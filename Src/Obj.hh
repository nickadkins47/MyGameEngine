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

class Obj : public Manager<Obj>
{
    public:

    static bool add(string cref obj_name, string cref model_name);

    static bool add(string cref obj_name, Model ptr model);

    delete_other_ops(Obj)

    glm::mat4 model_mat {1.0f};

    glm::vec3 get_position() const;
    void set_position(glm::vec3 cref position);
    void move_position(glm::vec3 cref position);

    void rotate(float deg, glm::vec3 cref axis);

    void scale(glm::vec3 cref factor);

    constexpr Model ptr get_model();
    
    void set_model(Model ptr model);

    void unset_model();

    Obj(); //do not manually call. always use _::add() instead

    protected:

    Model ptr model = nullptr;
};