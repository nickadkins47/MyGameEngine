/**
 *   @file: ButtonHandler.hh
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

//Keeps track of external inputs for any given type of GLFW Input
//Ex: Keyboard keys, Mouse buttons, Joystick movements, etc.
//Use operator[] to read the state of any given button, based on results
//from glfwPollEvents(). Plus one more bool for simply if the key is down
class ButtonHandler
{
    public:
    
    using GLFWInputCode = int;

    struct Input {
        bool act_press = false; //True if just got GLFW_PRESS action from glfwPollEvents()
        bool act_repeat = false; //True if just got GLFW_REPEAT action from glfwPollEvents()
        bool act_release = false; //True if just got GLFW_RELEASE action from glfwPollEvents()
        bool is_down = false; //True if key is currently down, determined from GLFW actions
    };

    ButtonHandler();

    deleteOtherOps(ButtonHandler)

    //Resets all Input actions to false
    void reset();

    //Registers that the button (input_code) just polled (action)
    void set(GLFWInputCode input_code, int action);

    //Accesses the input state for the specified key/button
    Input cref operator[](GLFWInputCode input_code);

    protected:

    unordered_map<GLFWInputCode, Input> inputs;
};