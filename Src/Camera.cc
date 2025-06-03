/**
 *   @file: Camera.cc
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#include "Camera.hh"
    
Camera::Camera() {}

void Camera::update()
{
    //RH Z-up Coord system
    float ref pitch = angle[0];
    float ref yaw = angle[1];
    lookDirF = {
        sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch))
    };
    lookDirF = glm::normalize(lookDirF);

    lookDirL = glm::normalize(glm::cross({0,0,1}, lookDirF));
    lookDirU = glm::normalize(glm::cross(lookDirF, lookDirL));

    bool movements[6] {
        keyboard_ptr->is_pressed(GLFW_KEY_W), keyboard_ptr->is_pressed(GLFW_KEY_S), // +/- Forward
        keyboard_ptr->is_pressed(GLFW_KEY_A), keyboard_ptr->is_pressed(GLFW_KEY_D), // +/- Left
        keyboard_ptr->is_pressed(GLFW_KEY_Q), keyboard_ptr->is_pressed(GLFW_KEY_E), // +/- Up
    };

    float move_speed = (
          (keyboard_ptr->is_pressed(GLFW_KEY_LEFT_SHIFT)  ) ? 0.5f
        : (keyboard_ptr->is_pressed(GLFW_KEY_LEFT_CONTROL)) ? 0.03125f
                                  /*Else, Default to*/ : 0.125f
    );
    
    pos += (
        move_speed * lookDirF * cast<float>(movements[0] - movements[1]) +
        move_speed * lookDirL * cast<float>(movements[2] - movements[3]) +
        move_speed * lookDirU * cast<float>(movements[4] - movements[5])
    );
}

void Camera::update_angle(GLFWwindow ptr window, int screen_width, int screen_height)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    float static old_xpos = cast<float>(screen_width) / 2.0f;
    float static old_ypos = cast<float>(screen_height) / 2.0f;
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

void Camera::set_proj_mat(float fov_degrees, int window_width, int window_height, float near_z, float far_z) {
    float const aspect_ratio = cast<float>(window_width) / cast<float>(window_height);
    proj_mat = glm::perspective(
        glm::radians(fov_degrees), aspect_ratio, near_z, far_z
    );
}
