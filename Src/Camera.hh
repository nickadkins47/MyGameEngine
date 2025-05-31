/**
 *   @file: Camera.hh
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#pragma once

#include "Globals.hh"
#include "InputHandler.hh"

class Camera
{
    public:

    Camera();
    ~Camera();

    deleteOtherOps(Camera)

    void update();

    void update_angle(float xpos, float ypos);

    //Return the View-Projection matrix for this camera
    glm::mat4 get_vp_mat() const;

    //Set the Projection Matrix based on the given parameters
    void set_proj_mat(float fov_degrees, int window_width, int window_height, float near_z, float far_z);
    
    //Pointer to keyboard InputHandler
    InputHandler ptr keyboardPtr = nullptr;

    bool first_mouse = false;

    //protected:

    glm::mat4 proj_mat {0};

    glm::vec3 pos {0,0,0}; //camera's position in x/y/z
    glm::vec3 angle {0,0,0}; //camera's angle in pitch/yaw/roll

    glm::vec3 lookDirF {0};
    glm::vec3 lookDirL {0};
    glm::vec3 lookDirU {0};

};