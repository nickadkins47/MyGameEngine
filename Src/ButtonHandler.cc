/**
 *   @file: ButtonHandler.cc
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#include "ButtonHandler.hh"

ButtonHandler::ButtonHandler() {}

void ButtonHandler::reset()
{
    for (auto ref [_, input] : inputs)
    {
        input.act_press = false;
        input.act_repeat = false;
        input.act_release = false;
    }
}

void ButtonHandler::set(GLFWInputCode keycode, int action)
{
    Input ref input = inputs[keycode];
    switch (action)
    {
        case GLFW_PRESS:   input.act_press = true;   break;
        case GLFW_REPEAT:  input.act_repeat = true;  break;
        case GLFW_RELEASE: input.act_release = true; break;
        default: break;
    }
    if (input.act_press)   input.is_down = true;
    if (input.act_release) input.is_down = false;
}

ButtonHandler::Input cref ButtonHandler::operator[](GLFWInputCode input_code)
{
    return inputs[input_code];
}
