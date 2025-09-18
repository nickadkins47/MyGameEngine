/**
 *   @file: Core.cc
 * @author: Nicholas Adkins
 *   @date: May 28 2025
 *  @brief: 
 */

#include <fstream>
#include <sstream>

#include "Core.hh"
#include "Engine.hh"

int main(int argc, char const *argv[])
{
    engine = new Engine();
    engine->initialize();

    //Camera Init

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

    //Light Cubes

    array<glm::vec3, 4> const light_cube_positions {
        glm::vec3( 3.0f,  3.0f,  3.0f),
        glm::vec3( 12.3f, -3.3f,  3.0f),
        glm::vec3(-4.0f,  20.0f,  3.0f),
        glm::vec3( 0.0f,  -10.0f,  3.0f)
    };

    for (auto ref pos : light_cube_positions)
    {
        Obj ref obj = engine->new_obj("Models/TutorialCube.obj", "Shaders/Temp");
        obj.model->winding_cw = true;
        obj.move_position(pos);
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

    Shader ref sh = engine->get_shader("Shaders/Default");
    sh.init_lights(1, 8);
    
    sh.uniform_i("dir_lights[0].mode", 1);
    sh.uniform_fv("dir_lights[0].direction", 3, ivec3(-1.0f));
    sh.uniform_fv("dir_lights[0].ambient", 3, ivec3(0.05f));
    sh.uniform_fv("dir_lights[0].diffuse", 3, ivec3(0.05f));
    sh.uniform_fv("dir_lights[0].specular", 3, ivec3(0.05f));

    for (int i = 0; i < 4; i++)
    {
        string const light = format("lights[{}].", i);

        sh.uniform_i(light+"mode", 1);

        //TEMP: position of lights[0] is handled in engine.run()
        sh.uniform_fv(light+"position", 3, glm::value_ptr(light_cube_positions[i]));

        sh.uniform_fv(light+"ambient", 3, ivec3(0.0f));
        sh.uniform_fv(light+"diffuse", 3, ivec3(0.5f));
        sh.uniform_fv(light+"specular", 3, ivec3(1.0f));

        sh.uniform_fv(light+"attenuation", 3, ivec3(0.025f, 0.05f, 1.0f));
    }

    sh.uniform_i("lights[0].mode", 2);
    sh.uniform_fv("lights[0].sl_direction", 3, ivec3(0.0f, 0.0f, -1.0f));
    sh.uniform_f("lights[0].sl_bright_rim", glm::cos(glm::radians(15.0f)));
    sh.uniform_f("lights[0].sl_dark_rim", glm::cos(glm::radians(20.0f)));

    {
        Obj ref obj = engine->new_obj("Models/FlatGround.obj", "Shaders/Default");
        obj.model->winding_cw = true;
        obj.move_position(glm::vec3{0.0f, 0.0f, -3.0f});
        obj.scale(glm::vec3{128.0f});
    }

    {
        Obj ref obj = engine->new_obj("Models/Backpack/backpack.obj", "Shaders/Default");
        obj.move_position(glm::vec3{10.0f, 10.0f, 0.0f});
    }

    {
        Obj ref obj = engine->new_obj("Models/Duck/duck.dae", "Shaders/Default");
        obj.move_position(glm::vec3{-10.0f, -10.0f, 0.0f});
        obj.scale(glm::vec3(0.0325f));
        obj.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    {
        Obj ref obj = engine->new_obj("Models/Spider/spider.obj", "Shaders/Default");
        obj.move_position(glm::vec3{10.0f, -10.0f, 0.0f});
        obj.scale(glm::vec3(0.0325f));
        obj.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    {
        Obj ref obj = engine->new_obj("Models/RotatingCube.3DS", "Shaders/Default");
        obj.model->meshes[0].textures.push_back(&engine->get_texture("Textures/awesomeface.png"));
        obj.move_position(glm::vec3{-10.0f, 10.0f, 0.0f});
        obj.scale(glm::vec3(0.0325f));
        obj.rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    /* //Quads

    //moved quad model to its own file

    Shader ref quad_sh = engine->get_shader("Shaders/Quad");
    for (Mesh ptr ref model : MyChunk::quad_models)
        model->shader = &quad_sh;

    //MCEng stuff

    load_cube_txts();

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

    //Run Engine (Loop)

    engine->run();

    //Shutdown

    engine->shutdown();
    return 0;
}

constexpr string get_file_path(string cref path)
{
    return "../../../../" + path;
}

optional<string> get_file_contents(string cref path)
{
    std::ifstream in_file(get_file_path(path));
    if (in_file.fail())
    {
        print("get_file_contents error: cannot find {}\n", path);
        return nullopt;
    }
    return (std::stringstream() << in_file.rdbuf()).str();
    //in_file goes out of scope -> close()
}