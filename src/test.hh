
#pragma once

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

#include <vector>
#include <array>

#include <SDL3/SDL.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "asdf.hh"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

bool update(SDL_Window* gSDLWindow, SDL_Renderer* gSDLRenderer);
void render(SDL_Window* gSDLWindow, SDL_Renderer* gSDLRenderer, glm::vec3& camPos, glm::vec3& camAng, std::array<bool,6>& movements);