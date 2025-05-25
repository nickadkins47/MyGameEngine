
#pragma once

#include "Globals.hh"
#include "Camera.hh"
#include "Keyboard.hh"
#include "Obj/Obj.hh"

namespace MGE {

    class EngineCore {
    public:
        EngineCore();
        ~EngineCore();

        deleteOtherOps(EngineCore)

        void run();
        void update();

        void add_obj(Obj const& o);
        void add_objs(Vector<Obj> const& o_vec);
        void render();

        void asdf();

    //protected:
        bool shutdown = false;

        Camera cam;
        Keyboard kbd;
        SDL_Window* window = nullptr;
        SDL_GPUDevice* gpu_device = nullptr;
        SDL_GPUShader* vert_shader = nullptr;
        SDL_GPUShader* frag_shader = nullptr;
        SDL_GPUGraphicsPipeline* graphics_pipeline = nullptr;
        SDL_GPUBuffer* vertex_buf = nullptr;
        SDL_GPUTransferBuffer* transfer_buf = nullptr;

        Vector<Obj> obj_list {};

        Int static inline const windowWidth = 1200;
        Int static inline const windowHeight = 900;
        Float static inline const fov = 90.0f;
        Float static inline const zNear = 0.1f;
        Float static inline const zFar = 100.0f;

        StringView static inline const engName = "MyGameEngine";

        SDL_GPUShaderFormat static inline const shaderFmt = SDL_GPU_SHADERFORMAT_SPIRV;

        //Initializes all SDL-related components.
        //Returns true if successful, false otherwise
        bool init_sdl();

        //Deactivates all SDL-related components.
        void quit_sdl();

        //given a path (relative to the project dir) and the Shader's CreateInfo
        //(minus the "code", "code_size", and "format" parameters), construct a SDL_GPUShader & return
        //a pointer to it (or to nullptr if it failed).
        SDL_GPUShader* get_shader(String const& path, SDL_GPUShaderCreateInfo info);

        //map vertex from world coords to screen coords
        //static Vec<3> mapVertex(Vec<3> vertex);

    };

    // the vertex input layout
    struct Vertex {
        float x, y, z;      //vec3 position
        float r, g, b, a;   //vec4 color
    };

    struct UniformBuffer {
        float time; // you can add other properties here
        //float z_buf[EngineCore::windowWidth][EngineCore::windowHeight]
    };
    static UniformBuffer timeUniform {};

    // a list of vertices
    static Vertex vertices[] {
        {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},     // top vertex
        {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f},   // bottom left vertex
        {0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f},    // bottom right vertex
        {-0.1f, 0.4f, 0.1f, 1.0f, 0.0f, 0.0f, 1.0f},    // top vertex
        {-0.6f, -0.6f, 0.1f, 1.0f, 1.0f, 0.0f, 1.0f},   // bottom left vertex
        {0.4f, -0.6f, 0.1f, 1.0f, 0.0f, 1.0f, 1.0f},    // bottom right vertex
    };
    static int vertices_len = 6;

}