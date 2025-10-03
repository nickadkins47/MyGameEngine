/**
 *   @file: Camera.hh
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Core.hh"

class Camera
{
    public:

    Camera();

    delete_other_ops(Camera)

    //Camera's look sensitivity
    float sensitivity = 0.125f;

    bool first_mouse = false; //TODO move/remove

    //Updates the camera's angle (pitch & yaw) based on given screen coordinates
    void update_angle(float xpos, float ypos);

    //Return the View-Projection matrix for this camera
    glm::mat4 get_vp_mat() const;

    //Set the Projection Matrix based on the given parameters
    void set_p_mat(float fov_degrees, float near_z, float far_z);

    glm::vec3 get_position() const;
    void set_position(glm::vec3 cref position);
    void move_position(glm::vec3 cref position);

    glm::mat4 p_mat {0.0f}; //projection matrix
    glm::mat4 v_mat {0.0f}; //view matrix

    glm::vec3 angle {0.0f}; //camera's angle in pitch/yaw/roll

    //NOTE: RH Z-up Coord system
    glm::vec3 look_dir_f {0.0f};
    glm::vec3 look_dir_l {0.0f};
    glm::vec3 look_dir_u {0.0f};

    //Recalculate look direction & also do angle constraints
    void recalculate_vecs();

    //Recalculate view matrix after pos or angle changes
    void recalculate_v_mat();

    protected:

    glm::vec3 pos {0.0f}; //camera's position in x/y/z

};