/**
 *   @file: Core.cc
 * @author: Nicholas Adkins
 *   @date: May 28 2025
 *  @brief: 
 */

//#include <angelscript.h>

#include "Core.hh"
#include "Engine.hh"
#include "MCEng/Grid.hh"

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
        /*Else, Default to*/
            : 0.125f
        ;
    };

    engine.is_selected_func = [&is_left_mouse_down, &is_tab_mode](){
        return is_left_mouse_down | is_tab_mode;
    };

    /*

    //Model(s)

    Model tutorial_cube {
        {
            // positions           // colors           // texture coords
            {3, GL_FLOAT},         {3, GL_FLOAT},      {2, GL_FLOAT}
        }, {
             0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right (up)
             0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right (up)
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left (up)
            -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left (up)
             0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right (down)
             0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right (down)
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left (down)
            -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left (down)
        }, {
            0,1,2, 2,3,0, //top
            4,7,6, 6,5,4, //4,5,6, 6,7,4, //bottom
            2,3,7, 7,6,2, //-x?
            0,1,5, 5,4,0, //+x?
            1,2,6, 6,5,1, //-y?
            0,3,7, 7,4,0, //+y?
        }
    };
    
    tutorial_cube.shader = new Shader("Shaders/TutorialCube.vert", "Shaders/TutorialCube.frag", 2);
    tutorial_cube.process();

    tutorial_cube.textures.push_back(Texture("Textures/container.jpg", GL_RGB));
    tutorial_cube.textures.push_back(Texture("Textures/awesomeface.png", GL_RGBA));

    //Objects

    glm::vec3 const axis1 = glm::normalize(glm::vec3(1.0f, 0.3f, 0.5f));

    array<glm::vec3, 10> const cube_positions {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f),
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
        //glm::vec3 pos = engine.objs[0].get_position();
        //engine.objs[0].set_position(pos + glm::vec3{0.1f, 0.1f, 0.1f});
        engine.objs[0].move_position(glm::vec3{0.1f, 0.1f, 0.1f});
    };

    engine.keyboard[GLFW_KEY_4].on_press = [&engine](){
        glm::vec3 pos = engine.objs[0].get_position();
        engine.objs[0].set_position(pos - glm::vec3{0.1f, 0.1f, 0.1f});
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

    //MCEng stuff

    load_cube_txts();

    Shader chunk_shader = Shader("Shaders/Chunk.vert", "Shaders/Chunk.frag", 1);

    MyChunk::shader = &chunk_shader;

    MyGrid grid;

    for (int cx = -10; cx < 10; cx++)
    {
        for (int cy = -10; cy < 10; cy++)
        {
            grid.load(cx,cy);
        }
    }

    //Run Engine (Loop)

    engine.run();

    //Extra

    return 0;
}  /// main