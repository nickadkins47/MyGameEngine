
#include "EngineCore.hh"

namespace MGE {

    EngineCore::EngineCore() {
        print("{}: Starting Up\n", eng_name);
        if (!init_sdl()) {
            print("{}\n", SDL_GetError());
            shutdown = true;
        }

        cam.kbdPtr = &kbd;
        cam.setProjMat(fov, window_width, window_height, near_z, far_z);
    }

    EngineCore::~EngineCore() {
        print("{}: Shutting Down\n", eng_name);
        quit_sdl();
    }

    void EngineCore::run() {
        while (!shutdown) update();
    }

    void EngineCore::update() {

        //handle any inputs from either mouse or keyboard
        if (SDL_Event e; SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                    shutdown = true; return;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    SDL_SetWindowRelativeMouseMode(window, true); break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    SDL_SetWindowRelativeMouseMode(window, false); break;
                case SDL_EVENT_MOUSE_MOTION:
                    if (SDL_GetWindowRelativeMouseMode(window)) {
                        cam.updateCamAngle(e.motion.xrel, e.motion.yrel);
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                    kbd.keyDown(e.key.key); break;
                case SDL_EVENT_KEY_UP:
                    kbd.keyUp(e.key.key); break;
                default: break;
            }
        }

        cam.update();

        render();
    }

    void EngineCore::add_obj(Obj const& o) {
        obj_list.push_back(o);
    }

    void EngineCore::add_objs(Vector<Obj> const& o_vec) {
        obj_list.append_range(o_vec);
    }

    void EngineCore::render() {
        Mat<4,4> const vpMat = cam.getVPMat();

        /* glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window); */

        /* SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer); 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);*/

        for (Obj const& o : obj_list) {
            Vector<Triangle<4>> newMesh {};

            for (Triangle<3> const& tri : o.mesh) {
                Triangle<4> newTri {Vec<4>{0,0,0,0}, Vec<4>{0,0,0,0}, Vec<4>{0,0,0,0}};
                bool withinClippingVolume = true;
                for (Int i = 0; i < tri.size(); i++) {
                    newTri[i] = vpMat * Vec<4>{ tri[i][0], tri[i][1], tri[i][2], 1.0f };
                    if (newTri[i][2] < near_z) {
                        withinClippingVolume = false;
                        break;
                    }
                    if (newTri[i][3] != 0.0f) { //if (std::fabs(newTri[i][3]) >= 0.0625f) {
                        newTri[i][0] /= newTri[i][3];
                        newTri[i][1] /= newTri[i][3];
                    }
                }
                if (withinClippingVolume) newMesh.push_back(newTri);
            }

            //TODO: implement proper clipping: https://gabrielgambetta.com/computer-graphics-from-scratch/11-clipping.html

            for (Int j = 0; j < newMesh.size(); j++) {
                Triangle<4>& newTri = newMesh[j];

                SDL_FPoint fpoints[] = {
                    {(newTri[0][0] + 1.0f) * (window_width/2), (newTri[0][1] + 1.0f) * (window_height/2)},
                    {(newTri[1][0] + 1.0f) * (window_width/2), (newTri[1][1] + 1.0f) * (window_height/2)},
                    {(newTri[2][0] + 1.0f) * (window_width/2), (newTri[2][1] + 1.0f) * (window_height/2)},
                    {(newTri[0][0] + 1.0f) * (window_width/2), (newTri[0][1] + 1.0f) * (window_height/2)},
                };
                //SDL_RenderLines(renderer, fpoints, 4);

                //print("Z's: {}, {}, {}\n", newTri[0][2], newTri[1][2], newTri[2][2]);

                SDL_Vertex vertices[] = {
                    //{fpoints[0], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                    //{fpoints[1], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                    //{fpoints[2], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                    {fpoints[0], {0.7f, 0.3f, 0.3f, 1.0f}, {0,0}},
                    {fpoints[1], {0.3f, 0.7f, 0.3f, 1.0f}, {0,0}},
                    {fpoints[2], {0.3f, 0.3f, 0.7f, 1.0f}, {0,0}},
                };
                //SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
                
            }
        }

        //SDL_RenderPresent(renderer);
        SDL_Delay(1);
        
    }

    bool EngineCore::init_sdl() {
        if (!SDL_Init(SDL_INIT_VIDEO)) return false;
        if (!SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 )) return false;

        if (!SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 )) return false;
        if (!SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 )) return false;
        if (!SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE )) return false;

        window = SDL_CreateWindow(eng_name.data(), window_width, window_height, SDL_WINDOW_OPENGL);
        if (window == NULL) return false;

        //SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"); //check if fail

        gl_context = SDL_GL_CreateContext(window);
        if (gl_context == NULL) return false;

        GLuint VAO;
        glGenVertexArrays(1, &VAO); // No crash here now!
        
        GLuint VBO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);

        GLuint vert_shader = load_shader("shaders/Vertex.vert", GL_VERTEX_SHADER);
        GLuint frag_shader = load_shader("shaders/Fragment.frag", GL_FRAGMENT_SHADER);

        GLuint gl_program = glCreateProgram();
        glAttachShader(gl_program, vert_shader);
        glAttachShader(gl_program, frag_shader);
        glLinkProgram(gl_program);
        
        GLint success;
        glGetProgramiv(gl_program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(gl_program, 1024, NULL, infoLog);
            print("Shader Linking Error: {}\n", infoLog);
        }

        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
        glEnableVertexAttribArray(1);

        return true;
    }
    
    void EngineCore::quit_sdl() {
        
        SDL_GL_DestroyContext(gl_context); //check if fail
        ifntn (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    GLuint load_shader(String const& shader_path, gl::GLenum const& shader_type) {
        std::ifstream shader_file(format("{}/{}", proj_dir, shader_path));
        if (shader_file.fail()) {
            /*???*/ return -1;
        }
        String shader_code = (std::stringstream() << shader_file.rdbuf()).str();
        char const* shader_code_c = shader_code.c_str();
        shader_file.close();

        GLuint shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &shader_code_c, NULL);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar info_log[1024];
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            print("Shader ({}) Compilation Error: {}\n", shader_path, info_log);
            return -1;
        }

        return shader;
    }

}