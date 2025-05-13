
#include "camera.hh"

glm::mat4 camUpdate(glm::vec3& camPos, glm::vec3& camAng, std::array<bool,6>& movements) {
    
    static glm::mat4 projMat = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    static bool isInverted = false;
    if (!isInverted) {
        projMat[1][1] = -projMat[1][1];
        isInverted = true;
    }

    //RH Z-up Coord system
    float& pitch = camAng[0];
    float& yaw = camAng[1];
    glm::vec3 lookDir (
        sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        -sin(glm::radians(pitch))
    );
    lookDir = glm::normalize(lookDir);

    glm::vec3 lookLeft = glm::normalize(glm::cross({0,0,1}, lookDir));
    glm::vec3 lookUp   = glm::normalize(glm::cross(lookDir, lookLeft));
    
    glm::mat4 viewMat = glm::lookAt(camPos, camPos + lookDir, lookUp);

    glm::mat4 vpMat = projMat * viewMat;

    //cam movements
    camPos += (
        lookDir  * (float)(movements[0] - movements[1]) * 0.0125f +
        lookLeft * (float)(movements[2] - movements[3]) * 0.0125f +
        lookUp   * (float)(movements[4] - movements[5]) * 0.0125f
    );

    return vpMat;
}