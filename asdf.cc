#include <SDL3/SDL.h>
//#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <glbinding/gl/gl.h>
using namespace gl;

void CheckShaderCompilation(GLuint shader, const char* type) {
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "Shader Compilation Error (" << type << "): " << infoLog << std::endl;
    }
}

void CheckShaderLinking(GLuint program) {
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cerr << "Shader Linking Error: " << infoLog << std::endl;
    }
}

GLuint LoadShader(const char* vertexPath, const char* fragmentPath) {
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    std::string vShaderCode = vShaderStream.str();
    std::string fShaderCode = fShaderStream.str();

    const char* vShaderSource = vShaderCode.c_str();
    const char* fShaderSource = fShaderCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderSource, NULL);
    glCompileShader(vertexShader);
    CheckShaderCompilation(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
    glCompileShader(fragmentShader);
    CheckShaderCompilation(fragmentShader, "FRAGMENT");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckShaderLinking(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


GLuint LoadTexture(const char* filepath) {
    stbi_set_flip_vertically_on_load(true);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (!data) {
        std::cerr << "Failed to load image: " << filepath << std::endl;
        return 0;
    }
    
    std::cout << "Loaded image: " << filepath << " (" << width << "x" << height << ")\n";

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

void CheckGLError(const char* function) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error in " << function << ": " << err << std::endl;
    }
}


// Main function
int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // Request OpenGL 3.3 Core Profile (Required on macOS)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("SDL3 Blur Shader", 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
    // Initialize GLEW (MUST be after OpenGL context creation)
    /* glewExperimental = GL_TRUE; // Fixes issues with some drivers
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Init Error: " << glewGetErrorString(err) << std::endl;
        SDL_GL_DestroyContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    } */

    

    std::cout << "GLEW initialized successfully!" << std::endl;
    
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;

    // Now it's safe to use OpenGL functions
    GLuint VAO;
    glGenVertexArrays(1, &VAO); // No crash here now!
    
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    // Load shaders
    GLuint shaderProgram = LoadShader("vertex_shader.glsl", "blur_shader.glsl");
    
    // Quad vertices
    float quadVertices[] = {
        // Positions   // TexCoords
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f,
            1.0f, -1.0f,   1.0f, 0.0f,
            1.0f,  1.0f,   1.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    GLuint texture = LoadTexture("lettuce.png");
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        CheckGLError("glUseProgram");

        glActiveTexture(GL_TEXTURE0);
        CheckGLError("glActiveTexture");

        glBindTexture(GL_TEXTURE_2D, texture);
        CheckGLError("glBindTexture");

        glUniform1i(glGetUniformLocation(shaderProgram, "screenTexture"), 0);
        CheckGLError("glUniform1i");

        glUniform2f(glGetUniformLocation(shaderProgram, "resolution"), 800.0f, 600.0f);
        CheckGLError("glUniform2f");

        glUniform1f(glGetUniformLocation(shaderProgram, "blurRadius"), 2.0f);
        CheckGLError("glUniform1f");

        glBindVertexArray(VAO);
        CheckGLError("glBindVertexArray");

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        CheckGLError("glDrawElements");

        // Ensure the buffer swap happens AFTER drawing
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}