/**
 *   @file: Shutdown.cc
 * @author: Nicholas Adkins
 *   @date: Oct 21 2025
 *  @brief: 
 */

#include <angelscript.h>
#include "../Ext/GLFW.hh"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../Engine.hh"

Engine::~Engine() {}

void Engine::shutdown()
{
    Log log("Shutting Down Engine");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //TODO: more detailed terminate?
    //also learn more about deleting certain specific things
    glfwTerminate();

    int r = 0;
    for (asIScriptFunction ptr func : engine->script_funcs)
    {
        if (func != nullptr)
        {
            r = func->Release();
            if (r < 0)
            {
                log.fail("func \"{}\" failed to release", func->GetName());
                return;
            }
        }
    }
    r = engine->script_engine->ShutDownAndRelease();
    if (r < 0)
    {
        log.fail("script shutdown function failed");
        return;
    }
}