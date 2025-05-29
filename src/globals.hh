
#pragma once

#include <array>
#include <format>
#include <print>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
#include <utility>

#include <fstream> //temp?
#include <sstream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <SDL3/SDL.h>
#include <glbinding/gl/gl.h>

using namespace gl;

//put this macro in a class (with the provided className) to delete the
//auto-generated copy/move constructor/assignment operators for this class
#define deleteOtherOps(className) \
    className( const className& toCopy ) = delete; \
    className( className&& toMove ) = delete; \
    className& operator=( const className& toCopyAssign ) = delete; \
    className& operator=( className&& toMoveAssign ) = delete;

//"if not null" check for if (thing) is not NULL or nullptr
#define ifntn(thing) if (thing != NULL && thing != nullptr)

namespace MGE {

    using Int = int;
    using Uint = unsigned int;
    using Float = float;
    using Size = size_t;

    template<typename T, Size S>
        using Array = std::array<T,S>;

    using std::format;

    using std::print;

    using String = std::string;

    using StringView = std::string_view;

    template<typename T>
        using Vector = std::vector<T>;

    template<Size T>
        using Vec = glm::vec<T, Float>;

    template<Size T, Size U>
        using Mat = glm::mat<T, U, Float>;

    template<Size T>
        using Triangle = Array<Vec<T>, 3>;

    template<typename T, typename S>
        using Pair = std::pair<T,S>;
    
    using std::make_pair;

    using glm::radians;

    using glm::normalize;

    using glm::cross;

    using glm::lookAt;

    using glm::perspective;

    //main directory, relative to GetBasePath, which is exe's directory, aka build/windows/x64/release
    String static const proj_dir = format("{}../../../..", SDL_GetBasePath());

    //wrapper for SDL_LoadFile; returns pointer to code & the size of code.
    //dont forget to free the void* later
    //str: path relative to project dir (Ex. Main is src/Main.cc)
    static inline std::tuple<void*, Size> load_file_as_code(String const& str) {
        Size codeSize;
        void* code = SDL_LoadFile(format("{}/{}", proj_dir, str).c_str(), &codeSize);
        if (code == NULL) {
            print("load_file_as_code ERROR: \"{}\" returned NULL\n{}\n", str, SDL_GetError());
            return {nullptr, 0};
        } else {
            return {code, codeSize};
        }
    }
    
}