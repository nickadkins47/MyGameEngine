/**
 *   @file: Obj.cc
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#include "Obj.hh"

Obj::Obj() {}

glm::vec3 Obj::get_position() const
{
    return glm::vec3{model_mat[3]};
}

void Obj::set_position(glm::vec3 const ref position)
{

}

void Obj::render(glm::mat4 const ref vp_mat) const
{
    glm::mat4 mvp_mat = vp_mat * model_mat; //cam's vp_mat * obj's model_mat

    set_uniform_mat(4f, (*model->shader), "mvp_mat", mvp_mat);
    
    model->render();
}