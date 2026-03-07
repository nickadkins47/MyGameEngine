/**
 *   @file: Initialize.cc
 * @author: Nicholas Adkins
 *   @date: Oct 21 2025
 *  @brief: 
 */

#include <AL/al.h>
#include <AL/alc.h>
#include <angelscript.h>
#include "../Ext/GL.hh"
#include <glbinding/glbinding.h>
#include "../Ext/GLFW.hh"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "ScriptMacros.hh"
#include "../VoxEng/Grid.hh"
#include "../ButtonHandler.hh"
#include "../Camera.hh"
#include "../Engine.hh"

//TODO: UPDATE maybe
//Info/Warn/Err Message Callback for Script Engine
void script_message_cb(asSMessageInfo cptr msg, void ptr param)
{
    /* if (msg->type == asMSGTYPE_INFORMATION)
    {
        Log::info("Script Engine [{}, {}, {}]: {}", 
            msg->section, msg->row, msg->col, msg->message
        );
    }
    else if (msg->type == asMSGTYPE_WARNING)
    {
        Log::warn("Script Engine [{}, {}, {}]: {}", 
            msg->section, msg->row, msg->col, msg->message
        );
    }
    else //else, Error
    {
        Log::error("Script Engine [{}, {}, {}]: {}", 
            msg->section, msg->row, msg->col, msg->message
        );
    } */
}

Engine::Engine() {}

void Engine::initialize()
{
    Log log("Initializing Engine");

    //GLFW Init

    if (!glfwInit())
    {
        log.fail("Couldn't initialize GLFW");
        shutdown();
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //v4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); //for multisampling

    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    log.info("Framebuffer Size: ({}, {})", window_width, window_height);

    window = glfwCreateWindow(window_width, window_height, window_name.data(), NULL, NULL);
    if (window == NULL)
    {
        log.fail("Couldn't initialize GLFW Window");
        shutdown();
        return;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    //OpenGL Init

    glbinding::initialize(glfwGetProcAddress);

    log.info("GL Version: {}", r_cast<char cptr>(glGetString(GL_VERSION)));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    if (!glfwExtensionSupported("GL_ARB_shader_draw_parameters"))
    {
        log.fail("GL_ARB_shader_draw_parameters not supported");
        shutdown();
    }
    
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    //ImGUI Init

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    //Script Engine Init

    std::ofstream predefs("as.predefined");
    if (predefs.fail())
    {
        log.fail("Couldn't open as.predefined");
        return;
    }

    engine->script_engine = asCreateScriptEngine();

    /*int r = engine->script_engine->SetMessageCallback(asFUNCTION(script_message_cb), 0, asCALL_CDECL);
    if (r < 0)
    {
        log.fail("Couldn't set message callback");
        return;
    } */

    script_funcs_init(log, predefs);
    
    predefs.close();


    //Init Audio
    ALCdevice ptr audio_device = alcOpenDevice(NULL);
    if (!audio_device)
    {
        //error here
    }

    ALCcontext ptr audio_context = alcCreateContext(audio_device, NULL);
    if (!audio_context)
    {
        //error here
    }

    if (!alcMakeContextCurrent(audio_context))
    {
        //error here
    }

    alListener3f(AL_POSITION, 0, 0, 0);
    // check for errors
    alListener3f(AL_VELOCITY, 0, 0, 0);
    // check for errors

    ALfloat listenerOri[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    alListenerfv(AL_ORIENTATION, listenerOri);
    // check for errors

    

    


    //Engine Components Init

    //vox_grid = new VoxGrid();
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
}