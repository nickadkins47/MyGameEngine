
#pragma once

//#include <optional>

#include "update.hh"

enum class QuadOrientation {
    XY, XZ, YZ
};

void renderQuad(glm::vec3 position, QuadOrientation orientation);

using triangle = array<glm::vec3, 3>;

inline vector<triangle> triangles {};

void render(SDL_Window* window, SDL_Renderer* renderer, glm::mat4& vpMat);
