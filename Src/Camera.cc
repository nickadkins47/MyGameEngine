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

    InputHandler ref kbd = engine->keyboard;
    bool movements[6] {
        kbd[GLFW_KEY_W].is_pressed, kbd[GLFW_KEY_S].is_pressed, // +/- Forward
        kbd[GLFW_KEY_A].is_pressed, kbd[GLFW_KEY_D].is_pressed, // +/- Left
        kbd[GLFW_KEY_Q].is_pressed, kbd[GLFW_KEY_E].is_pressed, // +/- Up
    };
    
    pos += (
        move_speed * lookDirF * cast<float>(movements[0] - movements[1]) +
        move_speed * lookDirL * cast<float>(movements[2] - movements[3]) +
        move_speed * lookDirU * cast<float>(movements[4] - movements[5])
    );
}

void Camera::update_angle()
{
    double xpos_d, ypos_d;
    glfwGetCursorPos(engine->window, &xpos_d, &ypos_d);
    float xpos = cast<float>(xpos_d);
    float ypos = cast<float>(ypos_d);

    float static old_xpos = cast<float>(engine->window_width) / 2.0f;
    float static old_ypos = cast<float>(engine->window_height) / 2.0f;

    if (!first_mouse)
    {
        //y,x is intentional here, as opposed to x,y.
        //forgot why angle[0] is being subtracted though.
        angle[0] -= (ypos - old_ypos) * sensitivity;
        angle[1] += (xpos - old_xpos) * sensitivity;
    }
    else first_mouse = false;

    old_xpos = xpos;
    old_ypos = ypos;

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
