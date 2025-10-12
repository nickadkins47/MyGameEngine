/**
 *   @file: GLFW.hh
 * @author: Nicholas Adkins
 *   @date: Oct 05 2025
 *  @brief: Special header to handle including GLFW (given that i use GLBinding)
 */

#pragma once

#include "GL.hh"

//IE, dont include the GL.h from windows kits
#ifndef GLFW_INCLUDE_NONE
    #define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
