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
#include <optional>
    using std::nullopt;
    using std::optional;
#include <string>
    using std::string;
#include <string_view>
    using std::string_view;
#include <unordered_map>
    using std::unordered_map;
#include <vector>
    using std::vector;

#define ptr *
#define ref &
#define mov &##&

#ifndef __GNUC__ //if not on Linux, no types.h, therefore define uint type here
    #define uint unsigned
#endif
#define cptr const ptr
#define cref const ref

#define cast static_cast
#define r_cast reinterpret_cast

//put this macro in a class (with the provided class_name) to delete the
//auto-generated copy/move constructor/assignment operators for this class
#define delete_other_ops(class_name) \
    class_name( class_name cref to_copy ) = delete; \
    class_name( class_name mov to_move ) = delete; \
    class_name ref operator=( class_name cref to_copy_assign ) = delete; \
    class_name ref operator=( class_name mov to_move_assign ) = delete;

#include "AuxFuncs.hh"
