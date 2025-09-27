/**
 *   @file: Light.hh
 * @author: Nicholas Adkins
 *   @date: Sep 23 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

class Obj;
class Shader;

class Light
{
    public:

    Light();

    //deleteOtherOps(Light)

    int mode = 0; //0: Ignore, 1: Normal Light, 2: Global Light, 3: Spotlight

    glm::vec3 diffuse {0.0f};
    glm::vec3 specular {0.0f};
    glm::vec3 ambient {0.0f};
    glm::vec3 attenuation {0.0f};
    glm::vec3 position {0.0f};
    glm::vec3 direction {0.0f};
    float bright_rim = 0.0f;
    float dark_rim = 0.0f;

    int follower_index = -1; //Set this to the index of an Obj for this Light to follow  

    //Updates the GPU's light[index] to match the one in this class
    void update(int index, Shader cptr shader) const;

    //Updates the position of the GPU's light[index] to match the one in this class
    void update_pos(int index, Shader cptr shader) const;

};
