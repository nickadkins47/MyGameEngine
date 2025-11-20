/**
 *   @file: Core.cc
 * @author: Nicholas Adkins
 *   @date: May 28 2025
 *  @brief: 
 */

#include "Ext/GL.hh"
#include "Ext/GLFW.hh"
#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VoxEng/Grid.hh"
#include "ButtonHandler.hh"
#include "Camera.hh"
#include "Core.hh"
#include "Engine.hh"
#include "Model.hh"
#include "Shader.hh"
#include "Texture.hh"

int main(int argc, char ** argv)
{
    init_program();
    engine = new Engine();

    vector<string_view> const args (argv, argv + argc); //check arguments
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i] == "-S")
        {
            engine->script_entrypoint = args.at(i + 1);
            i++;
        }
        else if (args[i] == "-L") 
            Log::init_logging();
    }

    engine->initialize();

    //Loading Assets

    {
        Model::add("Models/Backpack/backpack.obj");
        Model::add("Models/Duck/duck.dae", false, false);
        Model::add("Models/Spider/spider.obj");
        Model::add("Models/FlatGround.obj", true);
        Model::add("Models/TutorialCube.obj", true);

        Shader::add("Default",
            "Shaders/Default.vert", "Shaders/Default.geom", "Shaders/Default.frag", 8, 16);
        Shader::add("Instanced",
            "Shaders/Instanced.vert", "Shaders/Default.geom", "Shaders/Default.frag", 8, 16);
        Shader::add("VoxQuads",
            "Shaders/VoxQuads.vert", "Shaders/VoxQuads.geom", "Shaders/Default.frag", 8, 16);

        Texture::add("Textures/awesomeface.png", 1);
        Texture::add("Textures/container.jpg", 1);
        Texture::add("Textures/container2.png", 1);
        Texture::add("Textures/container2_specular.png", 2);
        Texture::add("Textures/grass_bottom.png", 1);
        Texture::add("Textures/grass_side.png", 1);
        Texture::add("Textures/grass_top.png", 1);

        Texture::add("Textures/test/test1.png", 1);
        Texture::add("Textures/test/test2.png", 1);
        Texture::add("Textures/test/test3.png", 1);
        Texture::add("Textures/test/test4.png", 1);
        Texture::add("Textures/test/test5.png", 1);
        Texture::add("Textures/test/test6.png", 1);
    }

    engine->default_shader = Shader::get("Default");

    //Light Cubes

    array<glm::vec3, 4> constexpr light_cube_positions {
        glm::vec3( 10.0f,  10.0f,  20.0f),
        glm::vec3( 12.3f, -3.3f,  20.0f),
        glm::vec3(-4.0f,  20.0f,  20.0f),
        glm::vec3( 0.0f,  -10.0f,  20.0f)
    };

    for (int i = 0; i < 4; i++)
    {
        engine->lights.emplace_back() = {
            .mode = 1,
            .diffuse = glm::vec3(1.0f),
            .specular = glm::vec3(1.0f),
            .ambient = glm::vec3(0.0f),
            .attenuation = glm::vec3(0.025f, 0.05f, 1.0f),
            .position = light_cube_positions[i],
        };
    }

    /* engine->lights[0].mode = 3;
    engine->lights[0].direction = glm::vec3(0.0f, 0.0f, -1.0f);
    engine->lights[0].bright_rim = glm::cos(glm::radians(20.0f));
    engine->lights[0].dark_rim = glm::cos(glm::radians(25.0f)); */

    int const dir_light_n = cast<int>(engine->lights.size());
    Light ref dir_light = engine->lights.emplace_back();
    dir_light = {
        .mode = 2,
        .diffuse = glm::vec3(0.5f),
        .specular = glm::vec3(0.5f),
        .ambient = glm::vec3(0.25f),
        .direction = glm::vec3(0.0f, 0.0f, -1.0f),
    };

    /* for (int i = 0; i < light_cube_positions.size(); i++)
    {
        Obj::add(format("Lightcube{}", i), "Models/TutorialCube.obj");
        Obj ptr obj = Obj::get(format("Lightcube{}", i));
        obj->move_position(light_cube_positions[i]);
    }
    engine->lights[0].follower = Obj::get("Lightcube0");

    engine->runtime_cbs.push_back([](){
        Obj ptr light_cube0 = engine->lights[0].follower;
        if (engine->keyboard->at(GLFW_KEY_3).is_down)
            light_cube0->move_position(glm::vec3{0.1f, 0.1f, 0.0f});
        if (engine->keyboard->at(GLFW_KEY_4).is_down)
            light_cube0->move_position(glm::vec3{-0.1f, -0.1f, 0.0f});
        if (engine->keyboard->at(GLFW_KEY_5).is_down)
            light_cube0->move_position(glm::vec3{0.0f, 0.0f, 0.1f});
        if (engine->keyboard->at(GLFW_KEY_6).is_down)
            light_cube0->move_position(glm::vec3{0.0f, 0.0f, -0.1f});
    }); */

    /* Obj::add("Ground", "Models/FlatGround.obj");
    Obj ptr ground = Obj::get("Ground");
    ground->move_position(glm::vec3{0.0f, 0.0f, -3.0f});
    ground->scale(glm::vec3{128.0f});

    Obj::add("Backpack", "Models/Backpack/backpack.obj");
    Obj ptr backpack = Obj::get("Backpack");
    backpack->move_position(glm::vec3{10.0f, 10.0f, 15.0f});

    Obj::add("Duck", "Models/Duck/duck.dae");
    Obj ptr duck = Obj::get("Duck");
    duck->move_position(glm::vec3{-10.0f, -10.0f, 15.0f});
    duck->scale(glm::vec3(0.0325f));
    duck->rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    Obj::add("Spider", "Models/Spider/spider.obj");
    Obj ptr spider = Obj::get("Spider");
    spider->move_position(glm::vec3{10.0f, -10.0f, 15.0f});
    spider->scale(glm::vec3(0.0325f));
    spider->rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f)); */

    //VoxEng stuff

    load_cube_txts();

    VoxGrid::shader = Shader::get("VoxQuads");
    
    VoxGrid::shader->uniform_i("vox_x_dim", VoxChunk::x_dim);
    VoxGrid::shader->uniform_i("vox_y_dim", VoxChunk::y_dim);
    VoxGrid::shader->uniform_i("vox_quad_size", VoxChunk::quad_size);
    
    VoxGrid::temp_tex = Texture::get("Textures/grass_top.png");
    
    engine->vox_grid->for_each_chunk([](int cx, int cy){
        //if (cx == cy) continue; //test
        engine->vox_grid->load(cx,cy);
    });

    engine->vox_grid->finalize();

    //Other Runtime Callbacks

    engine->runtime_cbs.push_back([](){
        auto ref kbd = engine->keyboard;
        bool movements[6] {
            kbd->at(GLFW_KEY_W).is_down, kbd->at(GLFW_KEY_S).is_down, // +/- Forward
            kbd->at(GLFW_KEY_A).is_down, kbd->at(GLFW_KEY_D).is_down, // +/- Left
            kbd->at(GLFW_KEY_Q).is_down, kbd->at(GLFW_KEY_E).is_down, // +/- Up
        };

        float move_speed = cast<float>(engine->delta_time) * (
            (kbd->at(GLFW_KEY_LEFT_SHIFT).is_down)?   20.0f :
            (kbd->at(GLFW_KEY_LEFT_CONTROL).is_down)? 5.0f :
            /*Default*/ 10.0f
        );

        engine->camera->move_position(move_speed * (
            engine->camera->look_dir_f * cast<float>(movements[0] - movements[1]) +
            engine->camera->look_dir_l * cast<float>(movements[2] - movements[3]) +
            engine->camera->look_dir_u * cast<float>(movements[4] - movements[5])
        ));
    });

    engine->runtime_cbs.push_back([](){
        static bool is_tab_mode = false, tab_available = true;
        
        if (engine->keyboard->at(GLFW_KEY_TAB).is_down)
        {
            if (tab_available)
            {
                is_tab_mode = !is_tab_mode;
                tab_available = false;
            }
        }
        else tab_available = true;

        if (is_tab_mode || engine->mouse_buttons->at(GLFW_MOUSE_BUTTON_LEFT).is_down)
        {
            glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            double xpos_d, ypos_d;
            glfwGetCursorPos(engine->window, &xpos_d, &ypos_d);
            engine->camera->update_angle(cast<float>(xpos_d), cast<float>(ypos_d));
        }
        else
        {
            glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            engine->camera->first_mouse = true;
        }
    });

    engine->runtime_cbs.push_back([](){
        bool static b = true;
        if (engine->keyboard->at(GLFW_KEY_R).act_press) b = !b;

        //has to be outside if block for some reason
        for (auto ref [_, shader] : Shader::get_map())
            shader.uniform_i("extra_flag", cast<int>(b));
        glfwSwapInterval(cast<int>(b));
    });

    engine->runtime_cbs.push_back([&dir_light, &dir_light_n](){
        bool static b = true;
        if (engine->keyboard->at(GLFW_KEY_F).act_press)
        {
            b = !b;
            dir_light.mode = b ? 2 : 0;
            for (auto ref [_, shader] : Shader::get_map())
            {
                shader.use();
                update_light(dir_light_n, &shader);
            }
        }
    });

    //Run Engine (Loop)

    engine->render_init();
    while (engine->is_running)
        engine->render_frame();

    //Shutdown

    engine->shutdown();
    return 0;
}
