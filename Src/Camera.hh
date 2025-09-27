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

    //Camera's look sensitivity
    float sensitivity = 0.125f;

    bool first_mouse = false; //TODO move/remove

    //Updates the camera's angle (pitch & yaw) based on given screen coordinates
    void update_angle(float xpos, float ypos);

    //Return the View-Projection matrix for this camera
    glm::mat4 get_vp_mat() const;

    //Set the Projection Matrix based on the given parameters
    void set_proj_mat(float fov_degrees, float near_z, float far_z);

    //protected:

    glm::mat4 proj_mat {0.0f};

    glm::vec3 pos {0.0f}; //camera's position in x/y/z
    glm::vec3 angle {0.0f}; //camera's angle in pitch/yaw/roll

    //NOTE: RH Z-up Coord system
    glm::vec3 lookDirF {0.0f};
    glm::vec3 lookDirL {0.0f};
    glm::vec3 lookDirU {0.0f};

    //Recalculate look direction & also do angle constraints
    void recalculate_vecs();

};