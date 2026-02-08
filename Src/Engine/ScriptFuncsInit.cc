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

    /* m_funcdef(void, VoidCallback, ()); */

    char const * _s_VoidCallback = "void VoidCallback()"; 
    log.info("Script Funcdef \"{}\"", _s_VoidCallback);
    r = engine->script_engine->RegisterFuncdef(_s_VoidCallback); 
    if (r < 0) { log.fail("Cannot register funcdef"); return; } 
    predefs << "funcdef " << _s_VoidCallback << ";\n";

    /* m_global_func_sp(
        void, kbd_on_press, [],
        (int token, asIScriptFunction ptr cb),
        (int token, VoidCallback @cb),
    {
        engine->script_funcs.push_back(cb);
        engine->runtime_cbs.push_back([cb, token](){
            if (engine->keyboard->at(token).is_down)
                engine->run_function_as(cb);
        });
    }); */

    char const * _s_kbd_on_press = "void kbd_on_press(int token, VoidCallback @cb)"; log.info("Script Global Func \"{}\"", _s_kbd_on_press); typedef void (*_t_kbd_on_press)(int token, asIScriptFunction * cb); _t_kbd_on_press static _f_kbd_on_press = [](int token, asIScriptFunction * cb){ engine->script_funcs.push_back(cb); engine->runtime_cbs.push_back([cb, token](){ if (engine->keyboard->at(token).is_down) engine->run_function_as(cb); }); }; r = engine->script_engine->RegisterGlobalFunction(_s_kbd_on_press, asFunctionPtr(_f_kbd_on_press), asCALL_CDECL); if (r < 0) { log.fail("Cannot register function"); return; } predefs << _s_kbd_on_press << ";\n";

    /* m_global_func(void, camera_proj_mat, [], (float fov_degrees, float near_z, float far_z), {
        engine->camera->set_p_mat(fov_degrees, near_z, far_z);
    }); */
    
    char const * _s_camera_proj_mat = "void camera_proj_mat(float fov_degrees, float near_z, float far_z)"; log.info("Script Global Func \"{}\"", _s_camera_proj_mat); typedef void (*_t_camera_proj_mat)(float fov_degrees, float near_z, float far_z); _t_camera_proj_mat static _f_camera_proj_mat = [](float fov_degrees, float near_z, float far_z){ engine->camera->set_p_mat(fov_degrees, near_z, far_z); }; r = engine->script_engine->RegisterGlobalFunction(_s_camera_proj_mat, asFunctionPtr(_f_camera_proj_mat), asCALL_CDECL); if (r < 0) { log.fail("Cannot register function"); return; } predefs << _s_camera_proj_mat << ";\n";

    /* m_global_func(void, camera_pos, [], (float x, float y, float z), {
        engine->camera->set_position(glm::vec3(x,y,z));
    }); */

    char const * _s_camera_pos = "void camera_pos(float x, float y, float z)"; log.info("Script Global Func \"{}\"", _s_camera_pos); typedef void (*_t_camera_pos)(float x, float y, float z); _t_camera_pos static _f_camera_pos = [](float x, float y, float z){ engine->camera->set_position(glm::vec3(x,y,z)); }; r = engine->script_engine->RegisterGlobalFunction(_s_camera_pos, asFunctionPtr(_f_camera_pos), asCALL_CDECL); if (r < 0) { log.fail("Cannot register function"); return; } predefs << _s_camera_pos << ";\n";

    /* m_global_func(void, exit, [], (), {
        glfwSetWindowShouldClose(engine->window, true);
    }); */

    char const * _s_exit = "void exit()"; log.info("Script Global Func \"{}\"", _s_exit); typedef void (*_t_exit)(); _t_exit static _f_exit = [](){ glfwSetWindowShouldClose(engine->window, true); }; r = engine->script_engine->RegisterGlobalFunction(_s_exit, asFunctionPtr(_f_exit), asCALL_CDECL); if (r < 0) { log.fail("Cannot register function"); return; } predefs << _s_exit << ";\n";

    /* m_global_func(void, option_draw_point, [], (), {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }); */

    char const * _s_option_draw_point = "void option_draw_point()"; log.info("Script Global Func \"{}\"", _s_option_draw_point); typedef void (*_t_option_draw_point)(); _t_option_draw_point static _f_option_draw_point = [](){ glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); }; r = engine->script_engine->RegisterGlobalFunction(_s_option_draw_point, asFunctionPtr(_f_option_draw_point), asCALL_CDECL); if (r < 0) { log.fail("Cannot register function"); return; } predefs << _s_option_draw_point << ";\n";

    /* m_global_func(void, option_draw_lines, [], (), {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }); */

    char const * _s_option_draw_lines = "void option_draw_lines()"; log.info("Script Global Func \"{}\"", _s_option_draw_lines); typedef void (*_t_option_draw_lines)(); _t_option_draw_lines static _f_option_draw_lines = [](){ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }; r = engine->script_engine->RegisterGlobalFunction(_s_option_draw_lines, asFunctionPtr(_f_option_draw_lines), asCALL_CDECL); if (r < 0) { log.fail("Cannot register function"); return; } predefs << _s_option_draw_lines << ";\n";

    /* m_global_func(void, option_draw_fill, [], (), {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }); */

    char const * _s_option_draw_fill = "void option_draw_fill()"; log.info("Script Global Func \"{}\"", _s_option_draw_fill); typedef void (*_t_option_draw_fill)(); _t_option_draw_fill static _f_option_draw_fill = [](){ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }; r = engine->script_engine->RegisterGlobalFunction(_s_option_draw_fill, asFunctionPtr(_f_option_draw_fill), asCALL_CDECL); if (r < 0) { log.fail("Cannot register function"); return; } predefs << _s_option_draw_fill << ";\n";

    /* m_global_func_sp(
        void, add_runtime_cb, [],
        (asIScriptFunction ptr cb),
        (VoidCallback @cb),
    {
        engine->script_funcs.push_back(cb);
        engine->runtime_cbs.push_back([cb](){
            engine->run_function_as(cb);
        });
    }); */

    char const * _s_add_runtime_cb = "void add_runtime_cb(VoidCallback @cb)"; log.info("Script Global Func \"{}\"", _s_add_runtime_cb); typedef void (*_t_add_runtime_cb)(asIScriptFunction * cb); _t_add_runtime_cb static _f_add_runtime_cb = [](asIScriptFunction * cb){ engine->script_funcs.push_back(cb); engine->runtime_cbs.push_back([cb](){ engine->run_function_as(cb); }); }; r = engine->script_engine->RegisterGlobalFunction(_s_add_runtime_cb, asFunctionPtr(_f_add_runtime_cb), asCALL_CDECL); if (r < 0) { log.fail("Cannot register function"); return; } predefs << _s_add_runtime_cb << ";\n";

    /* m_global_func(bool, kbd_key, [], (int token), {
        return engine->keyboard->at(token).is_down;
    }); */

    char const * _s_kbd_key = "bool kbd_key(int token)"; log.info("Script Global Func \"{}\"", _s_kbd_key); typedef bool (*_t_kbd_key)(int token); _t_kbd_key static _f_kbd_key = [](int token){ return engine->keyboard->at(token).is_down; }; r = engine->script_engine->RegisterGlobalFunction(_s_kbd_key, asFunctionPtr(_f_kbd_key), asCALL_CDECL); if (r < 0) { log.fail("Cannot register function"); return; } predefs << _s_kbd_key << ";\n";

}