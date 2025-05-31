/**
 *   @file: Globals.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include <print>
    using std::print;
#include <format>
    using std::format;
#include <fstream>
#include <sstream>
#include <string>
    using std::string;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ptr *
#define ref &

using GLFWInputCode = int;

//put this macro in a class (with the provided className) to delete the
//auto-generated copy/move constructor/assignment operators for this class
#define deleteOtherOps(className) \
    className( const className& toCopy ) = delete; \
    className( className&& toMove ) = delete; \
    className& operator=( const className& toCopyAssign ) = delete; \
    className& operator=( className&& toMoveAssign ) = delete;

//gets the path to the specified file
//from relative-to-project-directory
//to relative-to-executable-directory.
//Ex: ${project dir}/Src/Main.cc ==> ${executable dir}/../../../../Src/Main.cc
inline string get_file(string const ref path)
{
    return "../../../../" + path;
}

int inline screen_width  = 1200;
int inline screen_height = 900;