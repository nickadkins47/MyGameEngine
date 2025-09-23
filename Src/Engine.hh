/**
 *   @file: Engine.hh
 * @author: Nicholas Adkins
 *   @date: Jun 02 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"
#include "Camera.hh"
#include "InputHandler.hh"
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

    int window_width  = 1200;
    int window_height = 900;

    function<bool(void)> is_selected_func = [](){return false;};

    GLFWwindow ptr window = nullptr;
    Camera camera;
    ScriptEng script_engine;

    InputHandler keyboard {glfwGetKey};
    InputHandler mouse_buttons {glfwGetMouseButton};

    vector<Obj> objs;

    unordered_map<string, Model> model_map;
    unordered_map<string, Shader> shader_map;
    unordered_map<string, Texture> texture_map;

    //TODO: Add skybox
    glm::vec3 skybox_color = {0.2f, 0.3f, 0.3f};

    void initialize();
    void shutdown();

};

Engine inline ptr engine = nullptr; //static pointer to current/primary engine