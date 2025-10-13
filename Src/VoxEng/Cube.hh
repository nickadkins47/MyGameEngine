/**
 *   @file: Cube.hh
 * @author: Nicholas Adkins
 *   @date: Jun 15 2025
 *  @brief: 
 */

#pragma once

#include "../Core.hh"

class Texture;

template<typename T>
using six = array<T,6>;

//types of blocks present in game
enum class VoxCube: size_t {
    AIR,
    TEST,
    GRASS,
    DIRT
};

unordered_map<VoxCube, six<Texture ptr>> inline cube_txts;

void load_cube_txts();