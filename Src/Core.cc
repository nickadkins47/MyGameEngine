/**
 *   @file: Core.cc
 * @author: Nicholas Adkins
 *   @date: May 28 2025
 *  @brief: 
 */

#include "MCEng/Grid.hh"
#include "Core.hh"
#include "Engine.hh"

int main(int argc, char ** argv)
{
    //check arguments
    for (string cref arg : vector<string>{argv, argv + argc})
    {
        if (arg == "-L1") Log::init_logging(1);
        if (arg == "-L2") Log::init_logging(2);
        if (arg == "-L3") Log::init_logging(3);
    }

    engine = new Engine();
    engine->initialize();

    //Loading Assets
    
    {
        Model::add("Models/Backpack/backpack.obj");
        Model::add("Models/Duck/duck.dae", false, false);
        Model::add("Models/Spider/spider.obj");
        Model::add("Models/FlatGround.obj", true);
        Model::add("Models/TutorialCube.obj", true);

        Model::add("Models/Quads/nx.obj", true);
        Model::add("Models/Quads/px.obj", true);
        Model::add("Models/Quads/ny.obj", true);
        Model::add("Models/Quads/py.obj", true);
        Model::add("Models/Quads/nz.obj", true);
        Model::add("Models/Quads/pz.obj", true);

        Shader::add("Shaders/Default", 8, 16);
        Shader::add("Shaders/Temp");

        Texture::add("Textures/awesomeface.png");
        Texture::add("Textures/container.jpg");
        Texture::add("Textures/container2.png");
        Texture::add("Textures/container2_specular.png");
        Texture::add("Textures/grass_bottom.png");
        Texture::add("Textures/grass_side.png");
        Texture::add("Textures/grass_top.png");

        Texture::add("Textures/test/test1.png");
        Texture::add("Textures/test/test2.png");
        Texture::add("Textures/test/test3.png");
        Texture::add("Textures/test/test4.png");
        Texture::add("Textures/test/test5.png");
        Texture::add("Textures/test/test6.png");
    }

    //Light Cubes

    array<glm::vec3, 4> constexpr light_cube_positions {
        glm::vec3( 10.0f,  10.0f,  20.0f),
        glm::vec3( 12.3f, -3.3f,  20.0f),
        glm::vec3(-4.0f,  20.0f,  20.0f),
        glm::vec3( 0.0f,  -10.0f,  20.0f)
    };

    for (int i = 0; i < 4; i++)
    {
        Light ref light = engine->lights.emplace_back();
        light.mode = 1;
        light.diffuse = glm::vec3(0.5f);
        light.specular = glm::vec3(1.0f);
        light.ambient = glm::vec3(0.0f);
        light.attenuation = glm::vec3(0.025f, 0.05f, 1.0f);
        light.position = light_cube_positions[i];
    }

    engine->lights[0].mode = 3;
    engine->lights[0].direction = glm::vec3(0.0f, 0.0f, -1.0f);
    engine->lights[0].bright_rim = glm::cos(glm::radians(20.0f));
    engine->lights[0].dark_rim = glm::cos(glm::radians(25.0f));

    /* Light ref directional_light = engine->lights.emplace_back();
    directional_light.mode = 2;
    directional_light.diffuse = glm::vec3(0.5f);
    directional_light.specular = glm::vec3(0.5f);
    directional_light.ambient = glm::vec3(0.5f);
    directional_light.direction = glm::vec3(0.0f, 0.0f, -1.0f); */

    for (auto ref pos : light_cube_positions)
    {
        Obj ptr obj = engine->new_obj("Models/TutorialCube.obj", "Shaders/Temp");
        obj->move_position(pos);
    }
    engine->lights[0].follower_index = 0;

    engine->runtime_cbs.push_back([](){
        if (engine->keyboard[GLFW_KEY_3].is_down)
            engine->objs[0].move_position(glm::vec3{0.1f, 0.1f, 0.0f});
        if (engine->keyboard[GLFW_KEY_4].is_down)
            engine->objs[0].move_position(glm::vec3{-0.1f, -0.1f, 0.0f});
        if (engine->keyboard[GLFW_KEY_5].is_down)
            engine->objs[0].move_position(glm::vec3{0.0f, 0.0f, 0.1f});
        if (engine->keyboard[GLFW_KEY_6].is_down)
            engine->objs[0].move_position(glm::vec3{0.0f, 0.0f, -0.1f});
    });

    {
        Obj ptr obj = engine->new_obj("Models/FlatGround.obj", "Shaders/Default");
        obj->move_position(glm::vec3{0.0f, 0.0f, -3.0f});
        obj->scale(glm::vec3{128.0f});
    }

    {
        Obj ptr obj = engine->new_obj("Models/Backpack/backpack.obj", "Shaders/Default");
        obj->move_position(glm::vec3{10.0f, 10.0f, 15.0f});
    }

    {
        Obj ptr obj = engine->new_obj("Models/Duck/duck.dae", "Shaders/Default");
        obj->move_position(glm::vec3{-10.0f, -10.0f, 15.0f});
        obj->scale(glm::vec3(0.0325f));
        obj->rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    {
        Obj ptr obj = engine->new_obj("Models/Spider/spider.obj", "Shaders/Default");
        obj->move_position(glm::vec3{10.0f, -10.0f, 15.0f});
        obj->scale(glm::vec3(0.0325f));
        obj->rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    //MCEng stuff

    /* load_cube_txts();

    //TODO set quad_models

    for (auto cref quad_model : MyChunk::quad_model_names)
        engine->get_model(quad_model)->winding_cw = true;

    MyGrid grid;

    int const h_sz_x = MyGrid::sz_x / 2; //half of sz_x
    int const h_sz_y = MyGrid::sz_y / 2; //half of sz_y

    for (int cx = - h_sz_x; cx < h_sz_x; cx++)
    {
        for (int cy = - h_sz_y; cy < h_sz_y; cy++)
        {
            grid.load(cx,cy);
        }
    } */

    //Other Runtime Callbacks

    engine->runtime_cbs.push_back([](){
        auto ref kbd = engine->keyboard;
        bool movements[6] {
            kbd[GLFW_KEY_W].is_down, kbd[GLFW_KEY_S].is_down, // +/- Forward
            kbd[GLFW_KEY_A].is_down, kbd[GLFW_KEY_D].is_down, // +/- Left
            kbd[GLFW_KEY_Q].is_down, kbd[GLFW_KEY_E].is_down, // +/- Up
        };

        float move_speed = 
            (engine->keyboard[GLFW_KEY_LEFT_SHIFT].is_down)?   0.5f :
            (engine->keyboard[GLFW_KEY_LEFT_CONTROL].is_down)? 0.03125f :
            /*Default*/ 0.125f
        ;
        
        engine->camera.pos += move_speed * (
            engine->camera.lookDirF * cast<float>(movements[0] - movements[1]) +
            engine->camera.lookDirL * cast<float>(movements[2] - movements[3]) +
            engine->camera.lookDirU * cast<float>(movements[4] - movements[5])
        );
    });

    engine->runtime_cbs.push_back([](){
        static bool is_tab_mode = false, tab_available = true;
        
        if (engine->keyboard[GLFW_KEY_TAB].is_down)
        {
            if (tab_available)
            {
                is_tab_mode = !is_tab_mode;
                tab_available = false;
            }
        }
        else tab_available = true;

        if (is_tab_mode || engine->mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].is_down)
        {
            glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            double xpos_d, ypos_d;
            glfwGetCursorPos(engine->window, &xpos_d, &ypos_d);
            engine->camera.update_angle(cast<float>(xpos_d), cast<float>(ypos_d));
        }
        else
        {
            glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            engine->camera.first_mouse = true;
        }
    });

    //Run Engine (Loop)

    engine->run();

    //Shutdown

    engine->shutdown();
    Log::log_file.close();
    return 0;
}
