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
class VoxGrid;
class ButtonHandler;
class Camera;
class ScriptEng;
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

    ScriptEng ptr script_engine;
    VoxGrid ptr vox_grid;

    ButtonHandler ptr keyboard;
    ButtonHandler ptr mouse_buttons;
    Camera ptr camera;

    Shader ptr default_shader = nullptr;

    vector<Light> lights;

    //Callbacks for the engine to run every frame
    vector<function<void(void)>> runtime_cbs;

    //TODO: Add skybox
    glm::vec3 skybox_color = {0.2f, 0.3f, 0.3f};

    void run();
    void render();

    void initialize();
    void shutdown();

    void opt_vsync_enable();
    void opt_vsync_disable();

};

Engine inline ptr engine = nullptr; //global pointer to current/primary engine
