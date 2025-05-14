
#pragma once

#include <array>
#include <print>
#include <string>
#include <string_view>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <SDL3/SDL.h>

//put this macro in a class (with the provided className) to delete the
//auto-generated copy/move constructor/assignment operators for this class
#define deleteOtherOps(className) \
    className( const className& toCopy ) = delete; \
    className( className&& toMove ) = delete; \
    className& operator=( const className& toCopyAssign ) = delete; \
    className& operator=( className&& toMoveAssign ) = delete;

namespace MGE {

    using Int = int;
    using Float = float;

    template<typename T, size_t S>
        using Array = std::array<T,S>;

    using std::print;

    using String = std::string;

    using StringView = std::string_view;

    template<typename T>
        using Vector = std::vector<T>;

    template<size_t T>
        using Vec = glm::vec<T, Float>;

    template<size_t T, size_t U>
        using Mat = glm::mat<T, U, Float>;

    template<size_t T>
        using Triangle = Array<Vec<T>, 3>;

    using glm::radians;

    using glm::normalize;

    using glm::cross;

    using glm::lookAt;

    using glm::perspective;

}