/**
 *   @file: AuxFuncs.cc
 * @author: Nicholas Adkins
 *   @date: Sep 19 2025
 *  @brief: 
 */

#include <chrono>
#include <filesystem>
#include <sstream>

#include "Ext/GL/Enum.hh"
#include "Ext/GL/Functions.hh"
#include "Ext/GLFW.hh"

#include "AuxFuncs.hh"

void init_program()
{
    //Set current directory from ./Build/Bin -> ./Build
    std::filesystem::current_path("..");
}

optional<string> get_file_contents(string_view file_path)
{
    //Lack of logging here bc i prefer doing it in the
    //places where this function is used, rather than here
    std::ifstream in_file(file_path.data());
    if (in_file.fail())
    {
        return nullopt;
    }
    else return (std::stringstream() << in_file.rdbuf()).str();
    //in_file goes out of scope -> close()
}

optional<string> get_gl_error()
{
    switch (glGetError())
    {
        case GL_INVALID_ENUM:                  return "Invalid enum";
        case GL_INVALID_VALUE:                 return "Invalid value";
        case GL_INVALID_OPERATION:             return "Invalid operation";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid framebuffer operation";
        case GL_OUT_OF_MEMORY:                 return "Out of memory";
        case GL_STACK_UNDERFLOW:               return "Stack underflow";
        case GL_STACK_OVERFLOW:                return "Stack overflow";
        default: return nullopt; //GL_NO_ERROR
    }
}

void Log::init_logging()
{
    if (Log::enabled) return; //already set
    Log::enabled = true;

    auto now_utc = std::chrono::system_clock::now();
    auto now_local = std::chrono::zoned_time(std::chrono::current_zone(), now_utc);
    string log_f_name = format("_D-{:%F}_T-{:%r}_.log", now_local, now_local);
    for (char ref c : log_f_name)
        if (c == ':' || c == ' ') c = '-';

    Log::out_file.open("Logs/" + log_f_name);
    if (Log::out_file.fail())
    {
        printf("ERROR: FAILED to open log\n");
        Log::enabled = false;
    }
}

double Log::get_time()
{
    return glfwGetTime();
}
