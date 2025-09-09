/**
 *   @file: Obj.cc
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#include "Obj.hh"

Obj::Obj(Model ptr model)
: model(model) {}

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

void Obj::render(glm::mat4 cref vp_mat) const
{
    //cam's vp_mat * obj's model_mat
    glm::mat4 mvp_mat = vp_mat * model_mat;

    model->shader->uniform_fm("mvp_mat", 4,4, glm::value_ptr(mvp_mat));

    model->shader->uniform_fm("m_mat", 4,4, glm::value_ptr(model_mat));

    //model matrix, inverted transposed & reduced to a mat3
    glm::mat3 model_mat_itr = glm::mat3(glm::transpose(glm::inverse(model_mat)));

    model->shader->uniform_fm("m_mat_itr", 3,3, glm::value_ptr(model_mat_itr));
    
    model->render();
}
