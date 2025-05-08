
#pragma once

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

#include <SDL3/SDL.h>

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

bool update(SDL_Window* gSDLWindow, SDL_Renderer* gSDLRenderer);