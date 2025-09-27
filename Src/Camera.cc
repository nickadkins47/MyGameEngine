/**
 *   @file: Camera.cc
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#include "Camera.hh"
#include "Engine.hh"

Camera::Camera() {}

void Camera::update_angle(float xpos, float ypos)
{
    float static old_xpos = 0.0f;
    float static old_ypos = 0.0f;

    //TODO move this out maybe? IE the first_mouse thing
    if (!first_mouse)
    {
        //y,x is intentional here, as opposed to x,y.
        //forgot why pitch is being subtracted though.
        angle[0] -= (ypos - old_ypos) * sensitivity; //pitch
        angle[1] += (xpos - old_xpos) * sensitivity; //yaw
    }
    else first_mouse = false;

    old_xpos = xpos;
    old_ypos = ypos;

    recalculate_vecs();
}

glm::mat4 Camera::get_vp_mat() const
{
    return proj_mat * glm::lookAt(pos, pos + lookDirF, lookDirU); //proj_mat * view_mat
}

void Camera::set_proj_mat(float fov_degrees, float near_z, float far_z)
{
    float const aspect_ratio = 
        cast<float>(engine->window_width) / 
        cast<float>(engine->window_height);
    proj_mat = glm::perspective(
        glm::radians(fov_degrees), aspect_ratio, near_z, far_z
    );
}

void Camera::recalculate_vecs()
{
    float ref pitch = angle[0];
    float ref yaw = angle[1];
    //float ref roll = angle[2]; //TODO

    if (pitch < -89.0f) { //constrain pitch to (-90, 90)
        pitch = -89.0f;
    } else if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (yaw < 0.0f) { //yaw will wrap around if beyond (0, 360)
        yaw += 360.0f;
    } else if (yaw >= 360.0f) {
        yaw -= 360.0f;
    }

    lookDirF = glm::normalize(glm::vec3(
        glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(pitch))
    ));
    lookDirL = glm::normalize(glm::cross(glm::vec3{0.0f, 0.0f, 1.0f}, lookDirF));
    lookDirU = glm::normalize(glm::cross(lookDirF, lookDirL));
}