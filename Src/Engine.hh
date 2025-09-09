/**
 *   @file: Engine.hh
 * @author: Nicholas Adkins
 *   @date: Jun 02 2025
 *  @brief: 
 */

#pragma once

#include "MCEng/Grid.hh"
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

    Engine(string cref window_name = "MyGameEngine");

    ~Engine();

    deleteOtherOps(Engine)

    void run();

    //protected:

    string window_name;
    bool valid = true;

    int window_width  = 1200;
    int window_height = 900;

    function<bool(void)> is_selected_func = [](){return false;};

    GLFWwindow ptr window = nullptr;
    InputHandler keyboard {glfwGetKey};
    InputHandler mouse_buttons {glfwGetMouseButton};
    Camera camera;
    ScriptEng script_engine;

    vector<Obj> objs;
    //TODO: have a more efficient way to store objs/models (multimap?)
    //s.t. it minimizes the use of model::bind()

    glm::vec3 skybox_color = {0.2f, 0.3f, 0.3f};

    void initialize();
    void shutdown();

    void error(string cref error_message);

};

Engine inline ptr engine = nullptr; //static pointer to current/primary engine