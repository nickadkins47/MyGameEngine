/**
 *   @file: Engine.hh
 * @author: Nicholas Adkins
 *   @date: Jun 02 2025
 *  @brief: 
 */

#pragma once

#include "Globals.hh"
#include "Camera.hh"
#include "InputHandler.hh"
#include "Model.hh"
#include "Obj.hh"
#include "Shader.hh"
#include "Texture.hh"

class Engine
{
    public:

    Engine();
    ~Engine();

    deleteOtherOps(Engine)

    void add_obj(Obj const ref obj);

    void run();

    //protected:

    bool valid = true;

    int screen_width  = 1200;
    int screen_height = 900;

    bool is_left_mouse_down = false;
    bool is_tab_mode = false;
    bool tab_available = true;

    GLFWwindow ptr window = nullptr;
    InputHandler keyboard {glfwGetKey};
    InputHandler mouse_buttons {glfwGetMouseButton};
    Camera camera;

    vector<Model> models;
    vector<Obj> objs;

    glm::vec3 skybox_color = {0.2f, 0.3f, 0.3f};

    void initialize();
    void shutdown();

    void error(string const ref error_message);

};