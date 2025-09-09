/**
 *   @file: InputHandler.hh
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

using GLFWInputCode = int;

//Keeps track of external inputs for any given type of GLFW Input
//Ex: Keyboard keys, Mouse buttons, Joystick movements, etc.
//Can be given functions to execute when an input is pressed or released,
//and you can actively read if that given input is currently pressed.
class InputHandler
{
    public:
    
    InputHandler(function<int(GLFWwindow ptr, GLFWInputCode)> input_func);
    
    deleteOtherOps(InputHandler)

    //stores things to do upon pressing a given key
    struct Input {
        function<void(void)> on_press   = [](){return;};
        function<void(void)> on_release = [](){return;};
        bool is_pressed = false;
    };

    //return const reference to key 
    Input cref at(GLFWInputCode cref input_code) const;

    //return reference to key
    Input ref operator[](GLFWInputCode cref input_code);

    //handles checking keyboard input for a given window
    //if any given key is pressed, it triggers corresponding actions
    void update(GLFWwindow ptr window);

    protected:

    function<int(GLFWwindow ptr, GLFWInputCode)> input_func;

    std::unordered_map<GLFWInputCode, Input> inputs;
};