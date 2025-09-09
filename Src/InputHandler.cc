/**
 *   @file: InputHandler.cc
 * @author: Nicholas Adkins
 *   @date: May 30 2025
 *  @brief: 
 */

#include "InputHandler.hh"

InputHandler::InputHandler(function<int(GLFWwindow ptr, GLFWInputCode)> input_func)
{
    this->input_func = input_func;
}

InputHandler::Input cref InputHandler::at(GLFWInputCode cref input_code) const
{
    return inputs.at(input_code);
}

InputHandler::Input ref InputHandler::operator[](GLFWInputCode cref input_code)
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
