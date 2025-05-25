
#include "Camera.hh"

namespace MGE {
    
    Camera::Camera() {}
    Camera::~Camera() {}

    void Camera::update() {

        //RH Z-up Coord system
        Float& pitch = angle[0];
        Float& yaw = angle[1];
        lookDirF = {
            sin(radians(yaw)) * cos(radians(pitch)),
            cos(radians(yaw)) * cos(radians(pitch)),
            sin(radians(pitch))
        };
        lookDirF = normalize(lookDirF);

        lookDirL = normalize(cross({0,0,1}, lookDirF));
        lookDirU = normalize(cross(lookDirF, lookDirL));

        bool movements[6] {
            kbdPtr->isKeyDown(SDLK_W), kbdPtr->isKeyDown(SDLK_S), // +/- Forward
            kbdPtr->isKeyDown(SDLK_A), kbdPtr->isKeyDown(SDLK_D), // +/- Left
            kbdPtr->isKeyDown(SDLK_Q), kbdPtr->isKeyDown(SDLK_E), // +/- Up
        };
        pos += (
            lookDirF * static_cast<Float>(movements[0] - movements[1]) * 0.0125f +
            lookDirL * static_cast<Float>(movements[2] - movements[3]) * 0.0125f +
            lookDirU * static_cast<Float>(movements[4] - movements[5]) * 0.0125f
        );
    }

    void Camera::updateCamAngle(Float xrel, Float yrel) {
        angle[0] += yrel * 0.125f;
        angle[1] += xrel * 0.125f;

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

    Mat<4,4> Camera::getVPMat() const {
        return projMat * lookAt(pos, pos + lookDirF, lookDirU); //projMat * viewMat
    }

    void Camera::setProjMat(Float fov, Float window_width, Float window_height, Float near_z, Float far_z) {
        projMat = perspective(
            radians(fov), static_cast<Float>(window_width) / static_cast<Float>(window_height), near_z, far_z
        );
    }

}