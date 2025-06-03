/**
 *   @file: Engine.cc
 * @author: Nicholas Adkins
 *   @date: Jun 02 2025
 *  @brief: 
 */

#include "Engine.hh"

Engine::Engine() { initialize(); }

Engine::~Engine() { shutdown(); }

void Engine::add_obj(Obj const ref obj)
{
    //TODO
}

void Engine::run()
{
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(skybox_color.x, skybox_color.y, skybox_color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSetInputMode(window, GLFW_CURSOR,
            (is_left_mouse_down | is_tab_mode)
                ? GLFW_CURSOR_DISABLED
                : GLFW_CURSOR_NORMAL
        );

        mouse_buttons.update(window);
        keyboard.update(window);

        if (is_left_mouse_down | is_tab_mode)
        {
            camera.update_angle(window, screen_width, screen_height);
        }
        else camera.first_mouse = true;

        camera.update();

        glm::mat4 vp_mat = camera.get_vp_mat();

        for (Model const ref model : models)
        {
            model.bind();
            for (Obj const ref obj : objs)
            {
                obj.render(vp_mat);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
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

    window = glfwCreateWindow(screen_width, screen_height, "My Game Engine", NULL, NULL);
    if (window == NULL)
        return error("Failed to create GLFW window");
    
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    //GLAD Init

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return error("Failed to initialize GLAD");

    glEnable(GL_DEPTH_TEST);

    //Other callbacks setup

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        //screen_width = width;
        //screen_height = height;
    });
}

void Engine::shutdown()
{
    print("Game Engine: Shutting Down\n");

    glfwTerminate();

    //TODO: more detailed terminate?
    //also learn more about deleting certain specific things
}

void Engine::error(string const ref error_message)
{
    print("{}\n", error_message); //TODO print to stderr?
    shutdown();
}