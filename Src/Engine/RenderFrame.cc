/**
 *   @file: RenderFrame.cc
 * @author: Nicholas Adkins
 *   @date: Oct 21 2025
 *  @brief: 
 */

#include "../Ext/GLFW.hh"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../VoxEng/Grid.hh"
#include "../ButtonHandler.hh"
#include "../Camera.hh"
#include "../Engine.hh"
#include "../Model.hh"
#include "../Shader.hh"

void Engine::render_frame()
{
    double static constexpr one_second = 1.0;
    double static fps_prev_time = glfwGetTime();
    double static fps_cur_time = fps_prev_time;
    int static frame_count = 0;
    int static display_fps = 0;
    double static display_ms_frame = 0.0;

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

    if (vox_grid != nullptr) vox_grid->render(); //Render voxel grid

    for (auto cref [_, model] : Model::get_map()) //Render everything else
    {
        if (!model.visible) continue;

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

    //ImGui Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window); //update screen

    //Reset action flags (IE GLFW_PRESS, GLFW_RELEASE, etc)
    keyboard->reset();
    mouse_buttons->reset();
    
    //Check if Engine should keep running
    glfwPollEvents();
    is_running = !cast<bool>(glfwWindowShouldClose(window));
}