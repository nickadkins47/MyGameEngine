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

Engine::Engine() { initialize(); }

Engine::Engine(string cref window_name)
: window_name(window_name) { initialize(); }

Engine::~Engine() { shutdown(); }

void Engine::run()
{
    double prev_time = glfwGetTime();
    int frame_count = 0;
    int display_fps = 0;
    double display_ms_frame = 0.0;

    for (/**/; !glfwWindowShouldClose(window); glfwPollEvents())
    {
        //ImGui New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        //FPS Count Handler
        double current_time = glfwGetTime();
        frame_count++;
        if (current_time - prev_time >= 1.0)
        {
            display_fps = frame_count;
            display_ms_frame = 1000.0 / cast<double>(frame_count);
            //print("{} FPS, {} ms/frame\n", display_fps, display_ms_frame);
            
            prev_time += 1.0;
            frame_count = 0;
        }
        ImGui::Begin("FPS Counter");
        ImGui::Text("%i FPS", display_fps);
        ImGui::Text("%4.3lf ms/frame", display_ms_frame);
        ImGui::End();

        //General Calculations

        bool is_selected = is_selected_func();

        if (is_selected)
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

        glm::mat4 vp_mat = camera.get_vp_mat();

        //General Rendering

        glClearColor(skybox_color.x, skybox_color.y, skybox_color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Obj cref obj : objs)
        {
            obj.model->bind();
            obj.render(vp_mat);
        }

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

void Engine::initialize()
{
    print("Game Engine: Initializing\n");

    //GLFW Init
    
    if (!glfwInit())
        return error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //v3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, window_name.c_str(), NULL, NULL);
    if (window == NULL)
        return error("Failed to create GLFW window");
    
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    //GLAD Init

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return error("Failed to initialize GLAD");

    glEnable(GL_DEPTH_TEST);

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
        //window_width = width;
        //window_height = height;
    });

    //Subcomponents init

    camera.engine_ptr = this;
}

void Engine::shutdown()
{
    print("Game Engine: Shutting Down\n");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    //TODO: more detailed terminate?
    //also learn more about deleting certain specific things
}

void Engine::error(string cref error_message)
{
    print("Game Engine: {}\n", error_message); //TODO print to stderr?
    shutdown();
}