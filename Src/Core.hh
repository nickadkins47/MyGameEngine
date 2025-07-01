/**
 *   @file: Core.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include <array>
    using std::array;
#include <format>
    using std::format;
#include <fstream>
#include <functional>
    using std::function;
#include <print>
    using std::print;
#include <ranges>
#include <string>
    using std::string;
#include <unordered_map>
#include <utility>
    using std::pair;
#include <vector>
    using std::vector;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/matrix_decompose.hpp>

#define ptr *
#define ref &
#define mov &##&

#define uint unsigned
#define cptr const ptr
#define cref const ref

#define cast static_cast
#define r_cast reinterpret_cast

using GLFWInputCode = int;

//put this macro in a class (with the provided className) to delete the
//auto-generated copy/move constructor/assignment operators for this class
#define deleteOtherOps(class_name) \
    class_name( class_name cref toCopy ) = delete; \
    class_name( class_name mov toMove ) = delete; \
    class_name ref operator=( class_name cref toCopyAssign ) = delete; \
    class_name ref operator=( class_name mov toMoveAssign ) = delete;

//gets the path to the specified file
//from relative-to-project-directory
//to relative-to-executable-directory.
//Ex: ${project dir}/Src/Main.cc ==> ${executable dir}/../../../../Src/Main.cc
inline constexpr string get_file(string cref path)
{
    return "../../../../" + path;
}
