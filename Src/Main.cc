/**
 *   @file: Main.cc
 * @author: Nicholas Adkins
 *   @date: May 28 2025
 *  @brief: 
 */

#include "Globals.hh"
#include "Shader.hh"
#include "Texture.hh"
#include "InputHandler.hh"
#include "Camera.hh"

float vertices[] = {
    // positions           // colors           // texture coords
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right (up)
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right (up)
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left (up)
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left (up)
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right (down)
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right (down)
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left (down)
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left (down)
};   
unsigned int indices[] = {  // note that we start from 0!
    0,1,2, 2,3,0, //top
    4,7,6, 6,5,4, //4,5,6, 6,7,4, //bottom
    2,3,7, 7,6,2, //-x?
    0,1,5, 5,4,0, //+x?
    1,2,6, 6,5,1, //-y?
    0,3,7, 7,4,0, //+y?
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    screen_width = width;
    screen_height = height;
}

Camera camera;

bool is_left_mouse_down = false;
bool is_tab_mode = false;

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (is_left_mouse_down | is_tab_mode)
    {
        camera.update_angle(static_cast<float>(xpos), static_cast<float>(ypos));
    }
    else
    {
        camera.first_mouse = true;
    }
}

int main(int argc, char const *argv[])
{
    print("Game Engine: Starting Up\n");

    //GLFW Init
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Test", NULL, NULL);
    if (window == NULL)
    {
        print("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    //GLAD Init

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        print("Failed to initialize GLAD\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    //Textures

    Texture container_tex ("Textures/container.jpg", GL_RGB);
    Texture awesomeface_tex ("Textures/awesomeface.png", GL_RGBA);

    //Shader Importing

    Shader shader_prog ("Shaders/Vertex.vert", "Shaders/Fragment.frag");

    shader_prog.use();
    shader_prog.set_int("s_tex1", 0);
    shader_prog.set_int("s_tex2", 1);

    //Mouse Init

    InputHandler mouse (glfwGetMouseButton);
    
    mouse[GLFW_MOUSE_BUTTON_LEFT].on_press = [](){
        is_left_mouse_down = true;
    };
    mouse[GLFW_MOUSE_BUTTON_LEFT].on_release = [](){
        is_left_mouse_down = false;
    };

    //Keyboard Init

    InputHandler keyboard (glfwGetKey);

    keyboard[GLFW_KEY_ESCAPE].on_press = [window](){
        glfwSetWindowShouldClose(window, true);
    };

    keyboard[GLFW_KEY_1].on_press = [](){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    };
    keyboard[GLFW_KEY_2].on_press = [](){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    };

    bool tab_available = true;
    keyboard[GLFW_KEY_TAB].on_press = [&tab_available](){
        if (!tab_available) return;
        is_tab_mode = !is_tab_mode;
        tab_available = false;
    };
    keyboard[GLFW_KEY_TAB].on_release = [&tab_available](){
        tab_available = true;
    };

    //Camera Init

    camera.keyboardPtr = &keyboard;
    camera.set_proj_mat(90.0f, screen_width, screen_height, 0.1f, 100.0f);
    camera.pos = {0.0f, 0.0f, -3.0f};

    //Other callbacks setup

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);

    //Buffer Object Creation

    GLuint VAO; glGenVertexArrays(1, &VAO);
    GLuint VBO; glGenBuffers(1, &VBO);
    GLuint EBO; glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Main Loop

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_prog.use();

        glfwSetInputMode(window, GLFW_CURSOR, (is_left_mouse_down | is_tab_mode)
            ? GLFW_CURSOR_DISABLED
            : GLFW_CURSOR_NORMAL
        );

        keyboard.update(window);
        mouse.update(window);

        camera.update();

        GLuint vp_mat_loc = glGetUniformLocation(shader_prog.id, "vp_mat");
        glUniformMatrix4fv(vp_mat_loc, 1, GL_FALSE, glm::value_ptr(camera.get_vp_mat()));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container_tex.id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, awesomeface_tex.id);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model_mat = glm::mat4(1.0f);
            model_mat = glm::translate(model_mat, cubePositions[i]);
            float angle = 20.0f * i; 
            model_mat = glm::rotate(model_mat, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            GLuint model_mat_loc = glGetUniformLocation(shader_prog.id, "model_mat");
            glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(model_mat));

            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    print("Game Engine: Shutting Down\n");

    //Shutting Down

    glfwTerminate();

    return 0;
}  /// main