/**
 *   @file: Core.cc
 * @author: Nicholas Adkins
 *   @date: May 28 2025
 *  @brief: 
 */

//#include <angelscript.h>

#include "Core.hh"
#include "Engine.hh"

int main(int argc, char const *argv[])
{
    Engine engine;

    //Mouse Init

    bool is_left_mouse_down = false;

    engine.mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].on_press = [&is_left_mouse_down](){
        is_left_mouse_down = true;
    };
    engine.mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].on_release = [&is_left_mouse_down](){
        is_left_mouse_down = false;
    };

    //Keyboard Init

    engine.keyboard[GLFW_KEY_ESCAPE].on_press = [&engine](){
        glfwSetWindowShouldClose(engine.window, true);
    };

    engine.keyboard[GLFW_KEY_1].on_press = [](){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    };
    engine.keyboard[GLFW_KEY_2].on_press = [](){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    };

    bool is_tab_mode = false, tab_available = true;

    engine.keyboard[GLFW_KEY_TAB].on_press = [&is_tab_mode, &tab_available](){
        if (!tab_available) return;
        is_tab_mode = !is_tab_mode;
        tab_available = false;
    };
    engine.keyboard[GLFW_KEY_TAB].on_release = [&tab_available](){
        tab_available = true;
    };

    //Camera Init

    engine.camera.set_proj_mat(90.0f, 0.1f, 100.0f);
    engine.camera.pos = {0.0f, 0.0f, -3.0f};

    engine.camera.move_speed_func = [&engine](){
        return (engine.keyboard.is_pressed(GLFW_KEY_LEFT_SHIFT))
            ? 0.5f
        : (engine.keyboard.is_pressed(GLFW_KEY_LEFT_CONTROL))
            ? 0.03125f
        //Else, Default to
            : 0.125f
        ;
    };

    engine.is_selected_func = [&is_left_mouse_down, &is_tab_mode](){
        return is_left_mouse_down | is_tab_mode;
    };

    //Tutorial Cubes

    /* Model tutorial_cube {
        {
            //positions            //rgb colors        //texture coords
            {3, GL_FLOAT},         {3, GL_FLOAT},      {2, GL_FLOAT}
        }, {
            -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // top left (down)
            -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left (up)
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // bottom left (up)
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom left (down)

             0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom right (down)
             0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // bottom right (up)
             0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right (up)
             0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right (down)

            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left (down)
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left (up)
             0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right (up)
             0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right (down)

             0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // top right (down)
             0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top right (up)
            -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top left (up)
            -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // top left (down)

             0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom right (down)
             0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top right (down)
            -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top left (down)
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom left (down)
            
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left (up)
            -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left (up)
             0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right (up)
             0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right (up)
        }, {
             0,  1,  2,   2,  3,  0, //-x
             4,  5,  6,   6,  7,  4, //+x
             8,  9, 10,  10, 11,  8, //-y
            12, 13, 14,  14, 15, 12, //+y
            16, 17, 18,  18, 19, 16, //-z
            20, 21, 22,  22, 23, 20, //+z
        }
    };
    
    tutorial_cube.shader = new Shader("Shaders/TutorialCube.vert", "Shaders/TutorialCube.frag", 2);
    tutorial_cube.process();
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

    for (size_t i = 0; i < cube_positions.size(); i++)
    {
        Obj obj;
        obj.model = &tutorial_cube;
        obj.model_mat = glm::translate(obj.model_mat, cube_positions[i]);
        obj.model_mat = glm::rotate(obj.model_mat, glm::radians(20.0f * i), axis1);
        engine.objs.push_back(obj);
    }

    engine.keyboard[GLFW_KEY_3].on_press = [&engine](){
        engine.objs[0].move_position(glm::vec3{0.1f, 0.1f, 0.1f});
    };
    engine.keyboard[GLFW_KEY_4].on_press = [&engine](){
        engine.objs[0].move_position(glm::vec3{-0.1f, -0.1f, -0.1f});
    };

    engine.keyboard[GLFW_KEY_5].on_press = [&engine, &tutorial_cube, &axis1](){
        static float asdf = 1.0f;
        Obj new_obj;
        new_obj.model = &tutorial_cube;
        new_obj.model_mat = glm::translate(new_obj.model_mat, glm::vec3{-0.1f, 3.0f, asdf});
        new_obj.model_mat = glm::rotate(new_obj.model_mat, glm::radians(20.0f), axis1);
        engine.objs.push_back(new_obj);
        asdf += 2.0f;
    }; */

    //Quads

    MyChunk::quad_models = {
           new Model{ //-x
            {
                //positions           //texture coords
                {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                0.0f,  1.0f,  0.0f,   0.0f,  0.0f,
                0.0f,  1.0f,  1.0f,   0.0f,  1.0f,
                0.0f,  0.0f,  1.0f,   1.0f,  1.0f,
                0.0f,  0.0f,  0.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //+x
            {
                //positions           //texture coords
                {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                1.0f,  0.0f,  0.0f,   0.0f,  0.0f,
                1.0f,  0.0f,  1.0f,   0.0f,  1.0f,
                1.0f,  1.0f,  1.0f,   1.0f,  1.0f,
                1.0f,  1.0f,  0.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //-y
            {
                //positions           //texture coords
                {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                0.0f,  0.0f,  0.0f,   0.0f,  0.0f,
                0.0f,  0.0f,  1.0f,   0.0f,  1.0f,
                1.0f,  0.0f,  1.0f,   1.0f,  1.0f,
                1.0f,  0.0f,  0.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //+y
            {
                //positions           //texture coords
                {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                1.0f,  1.0f,  0.0f,   0.0f,  0.0f,
                1.0f,  1.0f,  1.0f,   0.0f,  1.0f,
                0.0f,  1.0f,  1.0f,   1.0f,  1.0f,
                0.0f,  1.0f,  0.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //-z
            {
                //positions           //texture coords
                {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                0.0f,  1.0f,  0.0f,   0.0f,  0.0f,
                0.0f,  0.0f,  0.0f,   0.0f,  1.0f,
                1.0f,  0.0f,  0.0f,   1.0f,  1.0f,
                1.0f,  1.0f,  0.0f,   1.0f,  0.0f,
            },{
                0,1,2, 2,3,0
            }
        }, new Model{ //+z
            {
                //positions           //texture coords
                {3, GL_FLOAT},        {2, GL_FLOAT}
            },{
                0.0f,  0.0f,  1.0f,   0.0f,  0.0f,
                0.0f,  1.0f,  1.0f,   0.0f,  1.0f,
                1.0f,  1.0f,  1.0f,   1.0f,  1.0f,
                1.0f,  0.0f,  1.0f,   1.0f,  0.0f,
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

    MyChunk::engine = &engine;

    MyGrid grid;
    engine.grid = &grid;

    int const h_sz_x = MyGrid::sz_x / 2; //half of sz_x
    int const h_sz_y = MyGrid::sz_y / 2; //half of sz_y

    for (int cx = - h_sz_x; cx < h_sz_x; cx++)
    {
        for (int cy = - h_sz_y; cy < h_sz_y; cy++)
        {
            grid.load(cx,cy);
        }
    }

    //Run Engine (Loop)

    engine.run();

    //Extra

    return 0;
}  /// main
