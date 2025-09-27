/**
 *   @file: Engine.hh
 * @author: Nicholas Adkins
 *   @date: Jun 02 2025
 *  @brief: 
 */

#pragma once

#include "ButtonHandler.hh"
#include "Core.hh"
#include "Camera.hh"
#include "Light.hh"
#include "Model.hh"
#include "Obj.hh"
#include "ScriptEng.hh"
#include "Shader.hh"
#include "Texture.hh"

class Engine
{
    public:

    Engine();

    ~Engine();

    deleteOtherOps(Engine)

    void run();

    Obj ptr new_obj(string cref model_name, string cref shader_name);

    //protected:

    string window_name = "MyGameEngine";
    bool valid = true;

    GLFWwindow ptr window = nullptr;
    int window_width  = 1200;
    int window_height = 900;

    ScriptEng script_engine;

    ButtonHandler keyboard;
    ButtonHandler mouse_buttons;
    Camera camera;

    vector<Obj> objs;
    vector<Light> lights;

    //unordered_map<string, Obj> obj_map; //TODO maybe?
    unordered_map<string, Model> model_map;
    unordered_map<string, Shader> shader_map;
    unordered_map<string, Texture> texture_map;

    //Callbacks for the engine to run every frame
    vector<function<void(void)>> runtime_cbs;

    //TODO: Add skybox
    glm::vec3 skybox_color = {0.2f, 0.3f, 0.3f};

    void initialize();
    void shutdown();

};

Engine inline ptr engine = nullptr; //static pointer to current/primary engine
