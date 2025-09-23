/**
 *   @file: Core.cc
 * @author: Nicholas Adkins
 *   @date: May 28 2025
 *  @brief: 
 */

#include "MCEng/Grid.hh"
#include "Core.hh"
#include "Engine.hh"

//inline glm::vec3 inside glm::value_ptr
#define ivec3(...) glm::value_ptr(glm::vec3(__VA_ARGS__))

int main(int argc, char ** argv)
{
    for (int i = 0; i < argc; i++)
    {
        string arg (argv[i]);
        if (arg == "-L1") Log::init_logging(1);
        if (arg == "-L2") Log::init_logging(2);
        if (arg == "-L3") Log::init_logging(3);
    }

    engine = new Engine();
    engine->initialize();

    //Loading Assets
    
    {
        Model::add("Models/Backpack/backpack.obj");
        Model::add("Models/Duck/duck.dae", false);
        Model::add("Models/Spider/spider.obj");
        Model::add("Models/FlatGround.obj");
        Model::add("Models/RotatingCube.3DS");
        Model::add("Models/TutorialCube.obj");

        Model::add("Models/Quads/nx.obj");
        Model::add("Models/Quads/px.obj");
        Model::add("Models/Quads/ny.obj");
        Model::add("Models/Quads/py.obj");
        Model::add("Models/Quads/nz.obj");
        Model::add("Models/Quads/pz.obj");

        Shader::add("Shaders/Default", 8);
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

    array<glm::vec3, 4> const light_cube_positions {
        glm::vec3( 3.0f,  3.0f,  20.0f),
        glm::vec3( 12.3f, -3.3f,  20.0f),
        glm::vec3(-4.0f,  20.0f,  20.0f),
        glm::vec3( 0.0f,  -10.0f,  20.0f)
    };

    Shader ptr sh = Shader::get("Shaders/Default").value();

    for (int i = 0; i < 4; i++)
    {
        sh->lights[i] = {
            .mode = 1,
            .ambient = glm::vec3(0.0f),
            .diffuse = glm::vec3(0.5f),
            .specular = glm::vec3(1.0f),
            .attenuation = glm::vec3(0.025f, 0.05f, 1.0f),
            .position = light_cube_positions[i],
        };
    }

    sh->lights[0].mode = 2;
    sh->lights[0].direction = glm::vec3(0.0f, 0.0f, -1.0f);
    sh->lights[0].bright_rim = glm::cos(glm::radians(20.0f));
    sh->lights[0].dark_rim = glm::cos(glm::radians(25.0f));

    /* sh->lights[4] = {
        .mode = 2,
        .ambient = glm::vec3(0.5f),
        .diffuse = glm::vec3(0.5f),
        .specular = glm::vec3(0.5f),
        .direction = glm::vec3(0.0f, 0.0f, -1.0f),
    }; */

    for (auto ref pos : light_cube_positions)
    {
        Obj ptr obj = engine->new_obj("Models/TutorialCube.obj", "Shaders/Temp");
        obj->model->winding_cw = true;
        obj->move_position(pos);
    }

    engine->keyboard[GLFW_KEY_3].on_press = [](){
        engine->objs[0].move_position(glm::vec3{0.1f, 0.1f, 0.0f});
    };
    engine->keyboard[GLFW_KEY_4].on_press = [](){
        engine->objs[0].move_position(glm::vec3{-0.1f, -0.1f, 0.0f});
    };
    engine->keyboard[GLFW_KEY_5].on_press = [](){
        engine->objs[0].move_position(glm::vec3{0.0f, 0.0f, 0.1f});
    };
    engine->keyboard[GLFW_KEY_6].on_press = [](){
        engine->objs[0].move_position(glm::vec3{0.0f, 0.0f, -0.1f});
    };

    {
        Obj ptr obj = engine->new_obj("Models/FlatGround.obj", "Shaders/Default");
        obj->model->winding_cw = true;
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

    {
        Obj ptr obj = engine->new_obj("Models/RotatingCube.3DS", "Shaders/Default");
        obj->model->meshes[0].textures.push_back(Texture::get("Textures/awesomeface.png").value());
        obj->move_position(glm::vec3{-10.0f, 10.0f, 15.0f});
        obj->scale(glm::vec3(0.0325f));
        obj->rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
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

    //Other

    engine->camera.move_speed_func = [](){
        return (engine->keyboard[GLFW_KEY_LEFT_SHIFT].is_pressed)
            ? 0.5f
        : (engine->keyboard[GLFW_KEY_LEFT_CONTROL].is_pressed)
            ? 0.03125f
        //Else, Default to
            : 0.125f
        ;
    };

    engine->is_selected_func = [](){
        static bool is_tab_mode = false, tab_available = true;
        
        if (engine->keyboard[GLFW_KEY_TAB].is_pressed)
        {
            if (tab_available)
            {
                is_tab_mode = !is_tab_mode;
                tab_available = false;
            }
        }
        else tab_available = true;

        return is_tab_mode | engine->mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].is_pressed;
    };

    //Run Engine (Loop)

    engine->run();

    //Shutdown

    engine->shutdown();
    Log::log_file.close();
    return 0;
}
