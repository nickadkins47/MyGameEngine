/**
 *   @file: Obj.cc
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#include <glm/gtc/matrix_transform.hpp>

#include "Engine.hh"
#include "Model.hh"
#include "Obj.hh"

Obj::Obj() {}

optional<Obj ptr> Obj::add(string cref obj_name, string cref model_name)
{
    Log log("Adding obj \"{}\"", obj_name);
    
    Obj ptr obj = new_val(obj_name);
    obj->set_model(Model::get(model_name));

    return obj;
}

optional<Obj ptr> Obj::add(string cref obj_name, Model ptr model)
{
    Log log("Adding obj \"{}\"", obj_name);
    
    Obj ptr obj = new_val(obj_name);
    obj->set_model(model);

    return obj;
}

glm::vec3 Obj::get_position() const
{
    return glm::vec3{model_mat[3]};
}

void Obj::set_position(glm::vec3 cref new_position)
{
    model_mat = glm::translate(model_mat, new_position - get_position());
}

void Obj::move_position(glm::vec3 cref movement)
{
    model_mat = glm::translate(model_mat, movement);
}

void Obj::rotate(float deg, glm::vec3 cref axis)
{
    model_mat = glm::rotate(model_mat, glm::radians(deg), axis);
}

void Obj::scale(glm::vec3 cref factor)
{
    model_mat = glm::scale(model_mat, factor);
}

constexpr Model ptr Obj::get_model()
{
    return model;
}

void Obj::set_model(Model ptr model)
{
    if (this->model != nullptr) unset_model();

    this->model = model;
    this->model->parent_objs.push_back(this);

    if (engine->is_running)
    {
        for (auto ref mesh : model->meshes)
        {
            if (mesh.IVBO != 0) //if instanced
                mesh.update_instance_m_mats();
        }
    }
}

void Obj::unset_model()
{
    auto iter = std::find(model->parent_objs.cbegin(), model->parent_objs.cend(), this);
    if (iter == model->parent_objs.cend()) return;
    model->parent_objs.erase(iter);

    if (engine->is_running)
    {
        for (auto ref mesh : model->meshes)
        {
            if (mesh.IVBO != 0) //if instanced
                mesh.update_instance_m_mats();
        }
    }

    this->model = nullptr;
}
