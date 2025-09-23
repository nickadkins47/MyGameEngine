/**
 *   @file: Camera.hh
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

class Camera
{
    public:

    Camera();

    deleteOtherOps(Camera)

    void update();

    void update_angle();

    //Return the View-Projection matrix for this camera
    glm::mat4 get_vp_mat() const;

    //Set the Projection Matrix based on the given parameters
    void set_proj_mat(float fov_degrees, float near_z, float far_z);

    //Pointer to parent game engine
    //Engine ptr engine_ptr = nullptr;

    bool first_mouse = false;

    float sensitivity = 0.125f;

    function<float(void)> move_speed_func = [](){return 1.0f;};

    //protected:

    glm::mat4 proj_mat {0};

    glm::vec3 pos {0,0,0}; //camera's position in x/y/z
    glm::vec3 angle {0,0,0}; //camera's angle in pitch/yaw/roll

    glm::vec3 lookDirF {0};
    glm::vec3 lookDirL {0};
    glm::vec3 lookDirU {0};

};