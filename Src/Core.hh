/**
 *   @file: Core.hh
 * @author: Nicholas Adkins
 *   @date: May 29 2025
 *  @brief: 
 */

#pragma once

#include <array>
    using std::array;
#include <filesystem>
    using std::filesystem::path;
#include <format>
    using std::format;
#include <fstream>
#include <functional>
    using std::function;
#include <optional>
    using std::nullopt;
    using std::optional;
#include <print>
    using std::print;
#include <string>
    using std::string;
#include <unordered_map>
    using std::unordered_map;
#include <vector>
    using std::vector;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ptr *
#define ref &
#define mov &##&

#define uint unsigned
#define cptr const ptr
#define cref const ref

#define cast static_cast
#define r_cast reinterpret_cast

//put this macro in a class (with the provided className) to delete the
//auto-generated copy/move constructor/assignment operators for this class
#define deleteOtherOps(class_name) \
    class_name( class_name cref toCopy ) = delete; \
    class_name( class_name mov toMove ) = delete; \
    class_name ref operator=( class_name cref toCopyAssign ) = delete; \
    class_name ref operator=( class_name mov toMoveAssign ) = delete;

#include "AuxFuncs.hh"
