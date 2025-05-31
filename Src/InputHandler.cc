/**
 *   @file: InputHandler.cc
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#include "InputHandler.hh"

InputHandler::InputHandler(std::function<int(GLFWwindow*, GLFWInputCode)> input_func)
{
    this->input_func = input_func;
}

InputHandler::~InputHandler() {}

InputHandler::Input const ref InputHandler::at(GLFWInputCode const ref input_code)
{
    return inputs[input_code];
}

InputHandler::Input ref InputHandler::operator[](GLFWInputCode const ref input_code)
{
    return inputs[input_code];
}

void InputHandler::update(GLFWwindow ptr window)
{
    for (auto & [input_code, input] : inputs)
    {
        if (input_func(window, input_code)) //GLFW_PRESS
        {
            input.on_press();
            input.is_pressed = true;
        }
        else //GLFW_RELEASE
        {
            input.on_release();
            input.is_pressed = false;
        }
    }
}

bool InputHandler::is_pressed(GLFWInputCode const ref input_code)
{
    return inputs[input_code].is_pressed;
}

bool InputHandler::is_not_pressed(GLFWInputCode const ref input_code)
{
    return !inputs[input_code].is_pressed;
}