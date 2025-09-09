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

    //Tutorial Cubes

    Model tutorial_cube {
        {
            //positions            //normals              //texture coords
            {3, GL_FLOAT},         {3, GL_FLOAT},         {2, GL_FLOAT}
        }, {
            -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,   // top left (down)
            -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   // top left (up)
            -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,   // bottom left (up)
            -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   // bottom left (down)

             0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,   // bottom right (down)
             0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,   // bottom right (up)
             0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,   // top right (up)
             0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,   // top right (down)

            -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,   // bottom left (down)
            -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,   // bottom left (up)
             0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,   // bottom right (up)
             0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,   // bottom right (down)

             0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,   // top right (down)
             0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,   // top right (up)
            -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,   // top left (up)
            -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,   // top left (down)

             0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,   // bottom right (down)
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,   // top right (down)
            -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,   // top left (down)
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,   // bottom left (down)
            
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   // bottom left (up)
            -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f,   // top left (up)
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,   // top right (up)
             0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f,   // bottom right (up)
        }, {
             0,  1,  2,   2,  3,  0, //-x
             4,  5,  6,   6,  7,  4, //+x
             8,  9, 10,  10, 11,  8, //-y
            12, 13, 14,  14, 15, 12, //+y
            16, 17, 18,  18, 19, 16, //-z
            20, 21, 22,  22, 23, 20, //+z
        }
    };
    
    tutorial_cube.process();
    tutorial_cube.shader = new Shader("Shaders/TutorialCube.vert", "Shaders/TutorialCube.frag", 2);
    tutorial_cube.textures.emplace_back("Textures/container.jpg");
    tutorial_cube.textures.emplace_back("Textures/awesomeface.png");

    glm::vec3 const axis1 = glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f));

    array<glm::vec3, 10> const cube_positions {
        glm::vec3( 3.0f,  3.0f,  3.0f ),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f,  12.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f ),
        glm::vec3(-1.7f,  3.0f, -7.5f ),
        glm::vec3( 5.3f, -2.0f, -2.5f ),
        glm::vec3( 1.5f, 11.0f, -5.5f ),
        glm::vec3(10.5f,  0.2f, -1.5f ),
        glm::vec3(-1.3f,  6.0f, -1.5f ),
    };

    {
        Obj obj (new Model(tutorial_cube));
        obj.move_position(cube_positions[0]);
        engine->objs.push_back(obj);
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

    //light.position is in engine.cc

    tutorial_cube.shader->uniform_fv("light.ambient", 3, ivec3(0.2f));
    tutorial_cube.shader->uniform_fv("light.diffuse", 3, ivec3(0.5f));
    tutorial_cube.shader->uniform_fv("light.specular", 3, ivec3(1.0f));

    tutorial_cube.shader->uniform_fv("material.ambient", 3, ivec3(1.0f, 0.5f, 0.31f));
    tutorial_cube.shader->uniform_fv("material.diffuse", 3, ivec3(1.0f, 0.5f, 0.31f));
    tutorial_cube.shader->uniform_fv("material.specular", 3, ivec3(0.5f, 0.5f, 0.5f));
    tutorial_cube.shader->uniform_f("material.shininess", 32.0f);

    /* for (size_t i = 1; i < cube_positions.size(); i++)
    {
        Obj obj (new Model(tutorial_cube));
        obj.move_position(cube_positions[i]);
        obj.rotate(20.0f * i, axis1);
        engine->objs.push_back(obj);
    } */

    Model flat_ground {
        {
            //positions           //normals              //texture coords
            {3, GL_FLOAT},        {3, GL_FLOAT},         {2, GL_FLOAT}
        }, {
            -0.5f, -0.5f, 0.0f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,   // top left
             0.5f,  0.5f, 0.0f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,   // bottom right
        }, {
            0,1,2, 2,3,0,
        }
    };

    flat_ground.process();
    flat_ground.shader = tutorial_cube.shader;
    
    {
        Obj obj (&flat_ground);
        obj.move_position(glm::vec3{0.0f, 0.0f, -3.0f});
        obj.scale(glm::vec3{128.0f});
        engine->objs.push_back(obj);
    }

    /* //Quads
    
    MyChunk::quad_models = {
           new Model{ //-x
            {
                //positions           //colors              //texture coords
                {3, GL_FLOAT},        {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,   0.0f,  0.0f,
                0.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,   0.0f,  1.0f,
                0.0f,  0.0f,  1.0f,   1.0f,  1.0f,  1.0f,   1.0f,  1.0f,
                0.0f,  0.0f,  0.0f,   1.0f,  1.0f,  1.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //+x
            {
                //positions           //colors              //texture coords
                {3, GL_FLOAT},        {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                1.0f,  0.0f,  0.0f,   1.0f,  1.0f,  1.0f,   0.0f,  0.0f,
                1.0f,  0.0f,  1.0f,   1.0f,  1.0f,  1.0f,   0.0f,  1.0f,
                1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,   1.0f,  1.0f,
                1.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //-y
            {
                //positions           //colors              //texture coords
                {3, GL_FLOAT},        {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                0.0f,  0.0f,  0.0f,   1.0f,  1.0f,  1.0f,   0.0f,  0.0f,
                0.0f,  0.0f,  1.0f,   1.0f,  1.0f,  1.0f,   0.0f,  1.0f,
                1.0f,  0.0f,  1.0f,   1.0f,  1.0f,  1.0f,   1.0f,  1.0f,
                1.0f,  0.0f,  0.0f,   1.0f,  1.0f,  1.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //+y
            {
                //positions           //colors              //texture coords
                {3, GL_FLOAT},        {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                1.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,   0.0f,  0.0f,
                1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,   0.0f,  1.0f,
                0.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,   1.0f,  1.0f,
                0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //-z
            {
                //positions           //colors              //texture coords
                {3, GL_FLOAT},        {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                0.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,   0.0f,  0.0f,
                0.0f,  0.0f,  0.0f,   1.0f,  1.0f,  1.0f,   0.0f,  1.0f,
                1.0f,  0.0f,  0.0f,   1.0f,  1.0f,  1.0f,   1.0f,  1.0f,
                1.0f,  1.0f,  0.0f,   1.0f,  1.0f,  1.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //+z
            {
                //positions           //colors              //texture coords
                {3, GL_FLOAT},        {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                0.0f,  0.0f,  1.0f,   1.0f,  1.0f,  1.0f,   0.0f,  0.0f,
                0.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,   0.0f,  1.0f,
                1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,   1.0f,  1.0f,
                1.0f,  0.0f,  1.0f,   1.0f,  1.0f,  1.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        },
    };

    Shader quad_sh ("Shaders/Quad.vert", "Shaders/Quad.frag", 1);
    for (Model ptr ref model : MyChunk::quad_models)
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

    //Extra Startup

    

    //Run Engine (Loop)

    engine->run();

    //Shutdown

    engine->shutdown();
    return 0;
}  /// main

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