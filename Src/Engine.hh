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

struct GLFWwindow;
class ButtonHandler;
class Camera;
class ScriptEng;

class Engine
{
    public:

    Engine();

    ~Engine();

    delete_other_ops(Engine)

    void run();

    Obj ptr new_obj(string cref model_name, string cref shader_name);

    //protected:

    string window_name = "MyGameEngine";
    bool valid = true;

    bool inline static opt_init_vsync = true; //temp?

    GLFWwindow ptr window = nullptr;
    int window_width  = 1200;
    int window_height = 900;

    ScriptEng ptr script_engine;

    ButtonHandler ptr keyboard;
    ButtonHandler ptr mouse_buttons;
    Camera ptr camera;

    vector<Obj> objs;
    vector<Light> lights;

    //Callbacks for the engine to run every frame
    vector<function<void(void)>> runtime_cbs;

    //TODO: Add skybox
    glm::vec3 skybox_color = {0.2f, 0.3f, 0.3f};

    void initialize();
    void shutdown();

};

Engine inline ptr engine = nullptr; //global pointer to current/primary engine
