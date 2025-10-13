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
    for (auto ref [_, button] : buttons)
    {
        button.act_press = false;
        button.act_repeat = false;
        button.act_release = false;
    }
}

void ButtonHandler::set(GLFWInputCode keycode, int action)
{
    Button ref button = buttons[keycode];
    switch (action)
    {
        case 1 /*GLFW_PRESS*/:   button.act_press = true;   break;
        case 2 /*GLFW_REPEAT*/:  button.act_repeat = true;  break;
        case 0 /*GLFW_RELEASE*/: button.act_release = true; break;
        default: break;
    }
    if (button.act_press)   button.is_down = true;
    if (button.act_release) button.is_down = false;
}

ButtonHandler::Button cref ButtonHandler::at(GLFWInputCode input_code)
{
    return buttons[input_code];
}
