/**
 *   @file: Engine.cc
 * @author: Nicholas Adkins
 *   @date: Jun 02 2025
 *  @brief: 
 */

#include "Engine.hh"

Engine::Engine() { initialize(); }

Engine::Engine(string cref window_name)
: window_name(window_name) { initialize(); }

Engine::~Engine() { shutdown(); }

void Engine::run()
{
    for (/**/; !glfwWindowShouldClose(window); glfwPollEvents())
    {
        bool is_selected = is_selected_func();

        glClearColor(skybox_color.x, skybox_color.y, skybox_color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        for (Obj cref obj : objs)
        {
            obj.model->bind();
            obj.render(vp_mat);
        }

        //TEMP MCEng stuff
        static const int offset = -15;
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
        }

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

    //Other callbacks setup

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

    glfwTerminate();

    //TODO: more detailed terminate?
    //also learn more about deleting certain specific things
}

void Engine::error(string cref error_message)
{
    print("Game Engine: {}\n", error_message); //TODO print to stderr?
    shutdown();
}