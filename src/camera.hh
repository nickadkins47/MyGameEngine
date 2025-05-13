
#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "update.hh"

static const int WINDOW_WIDTH = 1200;
static const int WINDOW_HEIGHT = 900;

static const float fov = 90.0f;
static const float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
static const float zNear = 0.1f;
static const float zFar = 100.0f;

//do camera stuff & return VPMatrix
glm::mat4 camUpdate(glm::vec3& camPos, glm::vec3& camAng, std::array<bool,6>& movements);
