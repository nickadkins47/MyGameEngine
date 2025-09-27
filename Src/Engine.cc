/**
 *   @file: Engine.cc
 * @author: Nicholas Adkins
 *   @date: Jun 02 2025
 *  @brief: 
 */

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Engine.hh"

Engine::Engine() {}

Engine::~Engine() {}

void Engine::run()
{
    //Init script stuff
    script_engine.run();

    //Init Lights
    for (auto cref [_, shader] : shader_map)
    {
        shader.use();
        for (int i = 0; i < lights.size(); i++)
            lights[i].update(i, &shader);
    }

    //Init FPS Counter
    double constexpr one_second = 1.0;
    double prev_time = glfwGetTime();
    int frame_count = 0;
    int display_fps = 0;
    double display_ms_frame = 0.0;

    //Main Engine Loop
    for (/**/; !glfwWindowShouldClose(window); glfwPollEvents())
    {
        if (!valid) return; //If game engine is to shut down, then break here

        //ImGui New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        //FPS Count Handler
        double current_time = glfwGetTime();
        frame_count++;
        if (current_time - prev_time >= one_second) //if a second has passed
        {
            display_fps = frame_count;
            display_ms_frame = 1000.0 / cast<double>(frame_count);
            prev_time += one_second;
            frame_count = 0;
        }
        ImGui::Begin("FPS Counter");
        ImGui::Text("%i FPS", display_fps);
        ImGui::Text("%4.3lf ms/frame", display_ms_frame);
        ImGui::End();

        //General Calculations

        if (is_selected_func())
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera.update_angle();
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            camera.first_mouse = true;
        }

        mouse_buttons.update(window);
        keyboard.update(window);
        camera.update();

        glm::mat4 const vp_mat = camera.get_vp_mat();

        for (auto cref [_, shader] : shader_map)
        {
            shader.use();
            shader.uniform_fv("view_pos", 3, glm::value_ptr(camera.pos));
        }

        //Update moving lights
        for (int i = 0; i < lights.size(); i++)
        {
            if (lights[i].follower_index >= 0)
            {
                lights[i].position = engine->objs[lights[i].follower_index].get_position();
                for (auto cref [_, shader] : shader_map)
                {
                    shader.use();
                    lights[i].update_pos(i, &shader);
                }
            }
        }

        //Rendering

        glClearColor(skybox_color.x, skybox_color.y, skybox_color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Obj cref obj : objs)
            obj.render(vp_mat);

        //MCEng Rendering (temp?)
        /* static const int offset = -15;
        if (grid != nullptr)
        {
            for (int cx = 0; cx < grid->sz_x; cx++)
            {
                for (int cy = 0; cy < grid->sz_y; cy++)
                {
                    MyChunk ref chunk = grid->chunk(cx,cy);
                    chunk.model->bind();
                    chunk.render(vp_mat);
                }
            }
        } */

        //ImGui Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); //update screen
    }
}

Obj ptr Engine::new_obj(string cref model_name, string cref shader_name)
{
    return &objs.emplace_back(Model::get(model_name).value(), Shader::get(shader_name).value());
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

    window = glfwCreateWindow(window_width, window_height, window_name.c_str(), NULL, NULL);
    if (window == NULL)
    {
        Log::error("Initializing: Failed (Couldn't initialize GLFW Window)");
        shutdown();
        return;
    }
    
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    //GLAD Init

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Log::error("Initializing: Failed (Couldn't initialize GLAD)");
        shutdown();
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //ImGUI Init

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    //Other callbacks Init

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow ptr window, int width, int height)
    {
        glViewport(0, 0, width, height);
        engine->window_width = width;
        engine->window_height = height;
    });

    /* glfwSetKeyCallback(window, [](GLFWwindow ptr window, int key, int scancode, int action, int mods)
    {

    }); */

    Log::info("Initializing: Success");
}

void Engine::shutdown()
{
    Log::info("Shutting Down");
    valid = false;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    //TODO: more detailed terminate?
    //also learn more about deleting certain specific things
}
