/**
 *   @file: Light.cc
 * @author: Nicholas Adkins
 *   @date: Sep 23 2025
 *  @brief: 
 */

#include "Light.hh"
#include "Shader.hh"

Light::Light() {}

void Light::update(int index, Shader cptr shader) const
{
    string const lname = format("lights[{}].", index);
    shader->uniform_i (lname+"mode"       , mode);
    shader->uniform_fv(lname+"diffuse"    , 3, glm::value_ptr(diffuse));
    shader->uniform_fv(lname+"specular"   , 3, glm::value_ptr(specular));
    shader->uniform_fv(lname+"ambient"    , 3, glm::value_ptr(ambient));
    shader->uniform_fv(lname+"attenuation", 3, glm::value_ptr(attenuation));
    shader->uniform_fv(lname+"position"   , 3, glm::value_ptr(position));
    shader->uniform_fv(lname+"direction"  , 3, glm::value_ptr(direction));
    shader->uniform_f (lname+"bright_rim" , bright_rim);
    shader->uniform_f (lname+"dark_rim"   , dark_rim);
}

void Light::update_pos(int index, Shader cptr shader) const
{
    shader->uniform_fv(format("lights[{}].position", index), 3, glm::value_ptr(position));
}
