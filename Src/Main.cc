/**
 *   @file: Main.cc
 * @author: Nicholas Adkins
 *   @date: May 28 2025
 *  @brief: 
 */

#include "Globals.hh"
#include "Engine.hh"

int main(int argc, char const *argv[])
{
    Engine engine;

    //Mouse Init

    engine.mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].on_press = [&engine](){
        engine.is_left_mouse_down = true;
    };
    engine.mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].on_release = [&engine](){
        engine.is_left_mouse_down = false;
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

    engine.keyboard[GLFW_KEY_TAB].on_press = [&engine](){
        if (!engine.tab_available) return;
        engine.is_tab_mode = !engine.is_tab_mode;
        engine.tab_available = false;
    };
    engine.keyboard[GLFW_KEY_TAB].on_release = [&engine](){
        engine.tab_available = true;
    };

    //Camera Init

    engine.camera.keyboard_ptr = &engine.keyboard;
    engine.camera.set_proj_mat(90.0f, engine.screen_width, engine.screen_height, 0.1f, 100.0f);
    engine.camera.pos = {0.0f, 0.0f, -3.0f};

    //Model(s)

    engine.models.push_back(Model {
        {
            {3, GL_FLOAT}, {3, GL_FLOAT}, {2, GL_FLOAT}
        }, {
            // positions           // colors           // texture coords
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
    });
    Model ref model = engine.models[0];
    
    model.shader = new Shader("Shaders/Vertex.vert", "Shaders/Fragment.frag", 2);
    model.process();

    model.textures.push_back(Texture("Textures/container.jpg", GL_RGB));
    model.textures.push_back(Texture("Textures/awesomeface.png", GL_RGBA));

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
        obj.model_mat = glm::translate(obj.model_mat, cube_positions[i]);
        obj.model_mat = glm::rotate(obj.model_mat, glm::radians(20.0f * i), axis1);
        obj.model = &model;
        engine.objs.push_back(obj);
    }

    //Run Engine

    engine.run();

    return 0;
}  /// main