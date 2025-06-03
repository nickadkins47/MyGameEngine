/**
 *   @file: Obj.hh
 * @author: Nicholas Adkins
 *   @date: May 31 2025
 *  @brief: 
 */

#pragma once

#include "Globals.hh"
#include "Model.hh"
#include "Shader.hh"

class Obj
{
    public:

    Obj();

    Model ptr model = nullptr;

    glm::vec3 get_position() const;
    void set_position(glm::vec3 const ref position);

    void render(glm::mat4 const ref vp_mat) const;

    //protected:
    
    glm::mat4 model_mat {1.0f};

};