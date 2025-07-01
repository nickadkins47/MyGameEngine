/**
 *   @file: Camera.cc
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#include "Camera.hh"
#include "Engine.hh"
    
Camera::Camera() {}

void Camera::update()
{
    //RH Z-up Coord system
    float ref pitch = angle[0];
    float ref yaw = angle[1];

    lookDirF = glm::normalize(glm::vec3(
        sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch))
    ));
    lookDirL = glm::normalize(glm::cross({0,0,1}, lookDirF));
    lookDirU = glm::normalize(glm::cross(lookDirF, lookDirL));

    float move_speed = move_speed_func();

    InputHandler ref kbd = engine_ptr->keyboard;
    bool movements[6] {
        kbd.is_pressed(GLFW_KEY_W), kbd.is_pressed(GLFW_KEY_S), // +/- Forward
        kbd.is_pressed(GLFW_KEY_A), kbd.is_pressed(GLFW_KEY_D), // +/- Left
        kbd.is_pressed(GLFW_KEY_Q), kbd.is_pressed(GLFW_KEY_E), // +/- Up
    };
    
    pos += (
        move_speed * lookDirF * cast<float>(movements[0] - movements[1]) +
        move_speed * lookDirL * cast<float>(movements[2] - movements[3]) +
        move_speed * lookDirU * cast<float>(movements[4] - movements[5])
    );
}

void Camera::update_angle()
{
    double xpos, ypos;
    glfwGetCursorPos(engine_ptr->window, &xpos, &ypos);

    float static old_xpos = cast<float>(engine_ptr->window_width) / 2.0f;
    float static old_ypos = cast<float>(engine_ptr->window_height) / 2.0f;
    float static const sensitivity = 0.125f;

    if (first_mouse)
    {
        old_xpos = cast<float>(xpos);
        old_ypos = cast<float>(ypos);
        first_mouse = false;
    }

    angle[0] -= (cast<float>(ypos) - old_ypos) * sensitivity; //inverted?
    angle[1] += (cast<float>(xpos) - old_xpos) * sensitivity;

    old_ypos = ypos;
    old_xpos = xpos;

    if (angle[0] < -89.0f) { //constrain pitch to (-90, 90)
        angle[0] = -89.0f;
    } else if (angle[0] > 89.0f) {
        angle[0] = 89.0f;
    }
    if (angle[1] < 0.0f) { //yaw will wrap around if beyond (0, 360)
        angle[1] += 360.0f;
    } else if (angle[1] >= 360.0f) {
        angle[1] -= 360.0f;
    }
}

glm::mat4 Camera::get_vp_mat() const {
    return proj_mat * glm::lookAt(pos, pos + lookDirF, lookDirU); //proj_mat * view_mat
}

void Camera::set_proj_mat(float fov_degrees, float near_z, float far_z) {
    float const aspect_ratio = 
        cast<float>(engine_ptr->window_width) / 
        cast<float>(engine_ptr->window_height);
    proj_mat = glm::perspective(
        glm::radians(fov_degrees), aspect_ratio, near_z, far_z
    );
}
