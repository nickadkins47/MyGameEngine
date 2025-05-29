
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

    //protected:
        bool shutdown = false;

        Camera cam;
        Keyboard kbd;
        SDL_Window* window = nullptr;
        //SDL_Renderer* renderer = nullptr;
        SDL_GLContext gl_context;

        GLuint gProgramID = 0;
        GLint gVertexPos2DLocation = -1;
        GLuint gVBO = 0;
        GLuint gIBO = 0;

        Vector<Obj> obj_list {};

        Int static inline const window_width = 1200;
        Int static inline const window_height = 900;
        Float static inline const fov = 90.0f;
        Float static inline const near_z = 0.1f;
        Float static inline const far_z = 100.0f;

        StringView static inline const eng_name = "MyGameEngine";

        SDL_GPUShaderFormat static inline const shader_fmt = SDL_GPU_SHADERFORMAT_SPIRV;

        //Initializes all SDL-related components.
        //Returns true if successful, false otherwise
        bool init_sdl();

        //Deactivates all SDL-related components.
        void quit_sdl();

    };

    //load shader from path (relative to proj_dir)
    GLuint load_shader(String const& shader_path, gl::GLenum const& shader_type);

    // Quad vertices
    float quadVertices[] = {
        // Positions   // TexCoords
        -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 0.0f,
            1.0f, -1.0f,   1.0f, 0.0f,
            1.0f,  1.0f,   1.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

}