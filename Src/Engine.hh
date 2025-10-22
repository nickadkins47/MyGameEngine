/**
 *   @file: Engine.hh
 * @author: Nicholas Adkins
 *   @date: Jun 02 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Light.hh"
#include "Obj.hh"

class asIScriptEngine;
class asIScriptFunction;
struct GLFWwindow;
class VoxGrid;
class ButtonHandler;
class Camera;
class Shader;

class Engine
{
    public:

    Engine();
    ~Engine();

    delete_other_ops(Engine)

    string window_name = "MyGameEngine";

    string script_entrypoint = "Scripts/Main.as";
    bool is_running = false;

    GLFWwindow ptr window = nullptr;
    int window_width  = 1200;
    int window_height = 900;

    double delta_time = 0; //time between this frame & previous

    asIScriptEngine ptr script_engine = nullptr;
    VoxGrid ptr vox_grid = nullptr;

    ButtonHandler ptr keyboard = nullptr;
    ButtonHandler ptr mouse_buttons = nullptr;
    Camera ptr camera = nullptr;

    Shader ptr default_shader = nullptr;

    vector<Light> lights;

    //Callbacks for the engine to run every frame
    vector<function<void(void)>> runtime_cbs;

    //TODO
    vector<asIScriptFunction ptr> script_funcs;

    //TODO: Add skybox
    glm::vec3 skybox_color = {0.2f, 0.3f, 0.3f};

    void initialize();

    void render_init();
    void render_frame();
    
    void shutdown();

    protected:

    void script_funcs_init(Log ref log, std::ofstream ref predefs);
    bool run_function_as(asIScriptFunction ptr cb);
};

Engine inline ptr engine = nullptr; //global pointer to current/primary engine
