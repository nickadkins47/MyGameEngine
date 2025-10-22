/**
 *   @file: ScriptFuncsInit.cc
 * @author: Nicholas Adkins
 *   @date: Oct 21 2025
 *  @brief: 
 */

#include <angelscript.h>
#include "../Ext/ScriptBuilder.hh"
#include "../Ext/GLFW.hh"

#include "ScriptMacros.hh"
#include "../ButtonHandler.hh"
#include "../Camera.hh"
#include "../Engine.hh"

void Engine::script_funcs_init(Log ref log, std::ofstream ref predefs)
{
    int r = 0;

    m_funcdef(void, VoidCallback, ());

    m_global_func_sp(
        void, kbd_on_press, [],
        (int token, asIScriptFunction ptr cb),
        (int token, VoidCallback @cb),
    {
        engine->script_funcs.push_back(cb);
        engine->runtime_cbs.push_back([cb, token](){
            if (engine->keyboard->at(token).is_down)
                engine->run_function_as(cb);
        });
    });

    m_global_func(void, camera_proj_mat, [], (float fov_degrees, float near_z, float far_z), {
        engine->camera->set_p_mat(fov_degrees, near_z, far_z);
    });

    m_global_func(void, camera_pos, [], (float x, float y, float z), {
        engine->camera->set_position(glm::vec3(x,y,z));
    });

    m_global_func(void, exit, [], (), {
        glfwSetWindowShouldClose(engine->window, true);
    });

    m_global_func(void, option_draw_point, [], (), {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    });

    m_global_func(void, option_draw_lines, [], (), {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    });

    m_global_func(void, option_draw_fill, [], (), {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    });

    m_global_func_sp(
        void, add_runtime_cb, [],
        (asIScriptFunction ptr cb),
        (VoidCallback @cb),
    {
        engine->script_funcs.push_back(cb);
        engine->runtime_cbs.push_back([cb](){
            engine->run_function_as(cb);
        });
    });

    m_global_func(bool, kbd_key, [], (int token), {
        return engine->keyboard->at(token).is_down;
    });

}