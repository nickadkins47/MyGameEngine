/**
 *   @file: Engine.cc
 * @author: Nicholas Adkins
 *   @date: Jun 02 2025
 *  @brief: 
 */

#include "Ext/GL.hh"
#include <glbinding/glbinding.h>
#include "Ext/GLFW.hh"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "ButtonHandler.hh"
#include "Camera.hh"
#include "Engine.hh"
#include "Model.hh"
#include "ScriptEng.hh"
#include "Shader.hh"
#include "Texture.hh"

Engine::Engine() {}

Engine::~Engine() {}

void Engine::run()
{
    is_running = true;

    //Init script stuff
    script_engine->run(script_entrypoint);

    //Init Lights
    for (auto cref [_, shader] : Shader::get_map())
    {
        shader.use();
        for (int i = 0; i < lights.size(); i++)
            update_light(i, &shader);
    }

    //Init Instanced Mesh data
    for (auto ref [_, model] : Model::get_map())
    {
        for (auto ref mesh : model.meshes)
        {
            const_cast<Mesh ref>(mesh).update_instance_m_mats();
        }
    }

    //Init FPS Counter
    double constexpr one_second = 1.0;
    double fps_prev_time = glfwGetTime();
    double fps_cur_time = fps_prev_time;
    int frame_count = 0;
    int display_fps = 0;
    double display_ms_frame = 0.0;

    //Init Camera values
    camera->update_angle(0.0f, 0.0f);

    //Main Engine Loop
    for (/**/; !glfwWindowShouldClose(window); glfwPollEvents())
    {
        frame_count++;
        delta_time = glfwGetTime() - fps_cur_time;
        fps_cur_time += delta_time;

        //ImGui New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        //FPS Count Handler
        if (fps_cur_time - fps_prev_time >= one_second) //if a second has passed
        {
            display_fps = frame_count;
            display_ms_frame = 1000.0 / cast<double>(frame_count);
            fps_prev_time += one_second;
            frame_count = 0;
        }
        ImGui::Begin("FPS Counter");
        ImGui::Text("%i FPS", display_fps);
        ImGui::Text("%4.3lf ms/frame", display_ms_frame);
        ImGui::End();

        //Run Callbacks
        for (auto cref cb : runtime_cbs)
            cb();

        //Update all shaders with certain values
        for (auto cref [_, shader] : Shader::get_map())
        {
            shader.use();
            shader.uniform_fv("view_pos", 3, glm::value_ptr(camera->get_position()));
            shader.uniform_fm("vp_mat", 4,4, glm::value_ptr(camera->get_vp_mat()));
        }

        //Update moving lights
        for (int i = 0; i < lights.size(); i++)
        {
            if (lights[i].follower != nullptr)
            {
                lights[i].position = lights[i].follower->get_position();
                for (auto cref [_, shader] : Shader::get_map())
                {
                    shader.use();
                    update_light_pos(i, &shader);
                }
            }
        }

        //Mesh/Model/Obj Rendering

        glClearColor(skybox_color.x, skybox_color.y, skybox_color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();

        //ImGui Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); //update screen

        //Reset action flags (IE GLFW_PRESS, GLFW_RELEASE, etc)
        keyboard->reset();
        mouse_buttons->reset();
    }
}

void Engine::render()
{
    for (auto cref [_, model] : Model::get_map())
    {
        Shader ptr shader = (model.shader == nullptr)
            ? default_shader
            : model.shader
        ;
        shader->use();

        glFrontFace(model.winding_cw ? GL_CW : GL_CCW);

        for (auto ref mesh : model.meshes)
        {
            mesh.set_textures(shader);
            
            if (model.instanced)
            {
                mesh.draw(); //model mat buffer (IVBO) should already be set
            }
            else
            {
                for (auto obj : model.parent_objs)
                {
                    shader->uniform_fm("m_mat", 4,4, glm::value_ptr(obj->model_mat));
                    mesh.draw();
                }
            }
        }
    }
}

void Engine::initialize()
{
    Log::info("Initializing...");

    //GLFW Init

    if (!glfwInit())
    {
        Log::error("Initializing: Failed (Couldn't initialize GLFW)");
        shutdown();
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //v3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); //for multisampling

    window = glfwCreateWindow(window_width, window_height, window_name.data(), NULL, NULL);
    if (window == NULL)
    {
        Log::error("Initializing: Failed (Couldn't initialize GLFW Window)");
        shutdown();
        return;
    }
    
    glfwMakeContextCurrent(window);

    opt_vsync_enable();

    //OpenGL Init

    glbinding::initialize(glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    //ImGUI Init

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    //Engine Components Init

    script_engine = new ScriptEng();
    keyboard = new ButtonHandler();
    mouse_buttons = new ButtonHandler();
    camera = new Camera();

    //Other callbacks Init

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow ptr window, int width, int height)
    {
        glViewport(0, 0, width, height);
        engine->window_width = width;
        engine->window_height = height;
    });

    glfwSetKeyCallback(window, [](GLFWwindow ptr window, int key, int scancode, int action, int mods)
    {
        engine->keyboard->set(key, action);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow ptr window, int button, int action, int mods)
    {
        engine->mouse_buttons->set(button, action);
    });

    Log::info("Initializing: Success");
}

void Engine::shutdown()
{
    Log::info("Shutting Down");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    //TODO: more detailed terminate?
    //also learn more about deleting certain specific things
}

void Engine::opt_vsync_enable()
{
    glfwSwapInterval(1);
}

void Engine::opt_vsync_disable()
{
    glfwSwapInterval(0);
}
