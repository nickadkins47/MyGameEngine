/**
 *   @file: Light.cc
 * @author: Nicholas Adkins
 *   @date: Sep 23 2025
 *  @brief: 
 */

#include <glm/gtc/type_ptr.hpp>

#include "Engine.hh"
#include "Light.hh"
#include "Shader.hh"

void update_light(int index, Shader cptr shader)
{
    string const lname = format("lights[{}].", index);
    Light cref light = engine->lights[index];
    shader->uniform_i (lname+"mode"       , light.mode);
    shader->uniform_fv(lname+"diffuse"    , 3, glm::value_ptr(light.diffuse));
    shader->uniform_fv(lname+"specular"   , 3, glm::value_ptr(light.specular));
    shader->uniform_fv(lname+"ambient"    , 3, glm::value_ptr(light.ambient));
    shader->uniform_fv(lname+"attenuation", 3, glm::value_ptr(light.attenuation));
    shader->uniform_fv(lname+"position"   , 3, glm::value_ptr(light.position));
    shader->uniform_fv(lname+"direction"  , 3, glm::value_ptr(light.direction));
    shader->uniform_f (lname+"bright_rim" , light.bright_rim);
    shader->uniform_f (lname+"dark_rim"   , light.dark_rim);
}

void update_light_pos(int index, Shader cptr shader)
{
    shader->uniform_fv(format("lights[{}].position", index), 3, glm::value_ptr(engine->lights[index].position));
}
