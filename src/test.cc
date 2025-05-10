
#include "test.hh"

bool update(SDL_Window* gSDLWindow, SDL_Renderer* gSDLRenderer) {

    static std::array<bool,6> movements {false};
    static glm::vec3 camPos {0.0f, 0.0f, 0.0f};
    static glm::vec3 camAng {0.0f, 0.0f, 0.0f};

    if (SDL_Event e; SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT:
                return false;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                SDL_SetWindowRelativeMouseMode(gSDLWindow, true); break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                SDL_SetWindowRelativeMouseMode(gSDLWindow, false); break;
            case SDL_EVENT_MOUSE_MOTION: {
                if (SDL_GetWindowRelativeMouseMode(gSDLWindow)) {
                    camAng[0] += e.motion.yrel * 0.0078125f;
                    camAng[1] += e.motion.xrel * 0.0078125f;
                }
                break;
            }
            case SDL_EVENT_KEY_DOWN: {
                switch (e.key.key) {
                    case SDLK_W: movements[0] = true; break;
                    case SDLK_S: movements[1] = true; break;
                    case SDLK_A: movements[2] = true; break;
                    case SDLK_D: movements[3] = true; break;
                    case SDLK_E: movements[4] = true; break;
                    case SDLK_Q: movements[5] = true; break;
                    
                    /* case SDLK_T: camAng[0] += 0.1f; break;
                    case SDLK_G: camAng[0] -= 0.1f; break;
                    case SDLK_H: camAng[1] += 0.1f; break;
                    case SDLK_F: camAng[1] -= 0.1f; break;
                    case SDLK_Y: camAng[2] += 0.1f; break;
                    case SDLK_R: camAng[2] -= 0.1f; break; */

                    default: break;
                }
                break;
            }
            case SDL_EVENT_KEY_UP: {
                switch (e.key.key) {
                    case SDLK_W: movements[0] = false; break;
                    case SDLK_S: movements[1] = false; break;
                    case SDLK_A: movements[2] = false; break;
                    case SDLK_D: movements[3] = false; break;
                    case SDLK_E: movements[4] = false; break;
                    case SDLK_Q: movements[5] = false; break;
        
                    case SDLK_ESCAPE: return false;
                    default: break;
                }
            }
            default: break;
        }
    }
    
    render(gSDLWindow, gSDLRenderer, camPos, camAng, movements);
    return true;
}

/* //radians
glm::mat4 rotX(float t) {
    return {
        1,      0,       0, 0,
        0, cos(t), -sin(t), 0,
        0, sin(t),  cos(t), 0,
        0,      0,       0, 1,
    };
}

//radians
glm::mat4 rotY(float t) {
    return {
         cos(t), 0, sin(t), 0,
              0, 1,      0, 0,
        -sin(t), 0, cos(t), 0,
              0, 0,      0, 1,
    };
}

//radians
glm::mat4 rotZ(float t) {
    return {
        cos(t), -sin(t), 0, 0,
        sin(t),  cos(t), 0, 0,
             0,       0, 1, 0,
             0,       0, 0, 1,
    };
} */

void render(SDL_Window* gSDLWindow, SDL_Renderer* gSDLRenderer, glm::vec3& camPos, glm::vec3& camAng, std::array<bool,6>& movements) {

    //WIP: got F & B, do the rest?
    std::vector<std::array<glm::vec3, 3>> mesh ({
        /* {glm::vec3 {0.1f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 1.0f}, glm::vec3 {1.0f, 0.0f, 1.0f}}, //F
        {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {1.0f, 0.0f, 1.0f}, glm::vec3 {1.0f, 0.0f, 0.0f}},

        {glm::vec3 {1.0f, 1.0f, 0.0f}, glm::vec3 {1.0f, 1.0f, 1.0f}, glm::vec3 {0.0f, 1.0f, 1.0f}}, //B
        {glm::vec3 {1.0f, 1.0f, 0.0f}, glm::vec3 {0.0f, 1.0f, 1.0f}, glm::vec3 {0.0f, 1.0f, 0.0f}}, */

        /* {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}}, //L
        {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}},

        {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}}, //R
        {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}},

        {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}}, //U
        {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}},
*/
        {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 1.0f, 0.0f}, glm::vec3 {1.0f, 1.0f, 0.0f}}, //D
        {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {1.0f, 1.0f, 0.0f}, glm::vec3 {1.0f, 0.0f, 0.0f}}, 
        {glm::vec3 {-1.0f, 0.0f, 0.0f}, glm::vec3 {-1.0f, 1.0f, 0.0f}, glm::vec3 {0.0f, 1.0f, 0.0f}}, //D
        {glm::vec3 {-1.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 1.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}}, 
        {glm::vec3 {-1.0f, -1.0f, 0.0f}, glm::vec3 {-1.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}}, //D
        {glm::vec3 {-1.0f, -1.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, -1.0f, 0.0f}}, 
        {glm::vec3 {0.0f, -1.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {1.0f, 0.0f, 0.0f}}, //D
        {glm::vec3 {0.0f, -1.0f, 0.0f}, glm::vec3 {1.0f, 0.0f, 0.0f}, glm::vec3 {1.0f, -1.0f, 0.0f}}, 

        {glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 1.0f}}, 
    });

    SDL_SetRenderDrawColor(gSDLRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gSDLRenderer);
    SDL_SetRenderDrawColor(gSDLRenderer, 255, 255, 255, 255);

    static float fov = 90.0f;
    static float aspect = WINDOW_WIDTH / WINDOW_HEIGHT;
    static float zNear = 0.1f;
    static float zFar = 100.0f;
    static glm::mat4 projMat = glm::perspective(fov, aspect, zNear, zFar);

/*     glm::mat4 t = glm::translate(glm::mat4(1.0), -camPos);
    glm::mat4 r = glm::mat4_cast(glm::quat(camAng));
    glm::mat4 viewMat = r * t; */

    glm::vec3 lookDir;
    lookDir.x = cos(camAng[0]) * cos(camAng[1]);
    lookDir.y = sin(camAng[1]);
    lookDir.z = cos(camAng[0]) * sin(camAng[1]);
    lookDir = glm::normalize(lookDir);
    glm::mat4 viewMat = glm::lookAt(camPos, camPos + lookDir, {0,0,1});

    glm::mat4 vpMat = projMat * viewMat;

    //cam movements
    glm::vec3 lookRight = glm::normalize(glm::cross({0,0,1}, lookDir));
    glm::vec3 lookUp = glm::cross(lookDir, lookRight);
    camPos += (
        lookDir   * (float)(movements[0] - movements[1]) * 0.0125f +
        lookRight * (float)(movements[2] - movements[3]) * 0.0125f +
        lookUp    * (float)(movements[4] - movements[5]) * 0.0125f
    );

    //TODO:
    //import my stuff from AftrBurner to here (IE MyKeyboard, MyCam, etc)

    for (auto tri : mesh) {
        std::array<glm::vec4, 3> newTri {glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0}, glm::vec4{0,0,0,0}};
        for (int i = 0; i < tri.size(); i++) {
            newTri[i] = vpMat * glm::vec4 { tri[i][0], tri[i][1], tri[i][2], 1.0f };
        }
        SDL_FPoint vertices[] = {
            {(newTri[0][0] + 1.0f) * (WINDOW_WIDTH/2), (newTri[0][1] + 1.0f) * (WINDOW_HEIGHT/2)},
            {(newTri[1][0] + 1.0f) * (WINDOW_WIDTH/2), (newTri[1][1] + 1.0f) * (WINDOW_HEIGHT/2)},
            {(newTri[2][0] + 1.0f) * (WINDOW_WIDTH/2), (newTri[2][1] + 1.0f) * (WINDOW_HEIGHT/2)},
            {(newTri[0][0] + 1.0f) * (WINDOW_WIDTH/2), (newTri[0][1] + 1.0f) * (WINDOW_HEIGHT/2)},
        };
        //SDL_RenderGeometry(gSDLRenderer, NULL, vertices, 4, NULL, 0);
        SDL_RenderLines(gSDLRenderer, vertices, 4);
    }

    /* SDL_Vertex vertices_1[] = {
        {{300, 300},{1.0, 0.0, 0.0, 1.0}},
        {{500, 300},{1.0, 0.0, 0.0, 1.0}},
        {{500, 500},{1.0, 0.0, 0.0, 1.0}},
        {{300, 500},{1.0, 0.0, 0.0, 1.0}},
    };
    const int indices_1[] = { 0,1,2, 2,3,0 };

    SDL_RenderGeometry(gSDLRenderer, NULL, vertices_1, 4, indices_1, 6);

    SDL_Vertex vertices_2[] = {
        {{300, 300},{0.0, 1.0, 0.0, 1.0}},
        {{400, 200},{0.0, 1.0, 0.0, 1.0}},
        {{600, 200},{0.0, 1.0, 0.0, 1.0}},
        {{500, 300},{0.0, 1.0, 0.0, 1.0}},
    };
    const int indices_2[] = { 0,1,2, 2,3,0 };

    SDL_RenderGeometry(gSDLRenderer, NULL, vertices_2, 4, indices_2, 6);

    SDL_Vertex vertices_3[] = {
        {{500, 300},{0.0, 0.0, 1.0, 1.0}},
        {{600, 200},{0.0, 0.0, 1.0, 1.0}},
        {{600, 400},{0.0, 0.0, 1.0, 1.0}},
        {{500, 500},{0.0, 0.0, 1.0, 1.0}},
    };
    const int indices_3[] = { 0,1,2, 2,3,0 };

    SDL_RenderGeometry(gSDLRenderer, NULL, vertices_3, 4, indices_3, 6);

    //basic triangle
    SDL_Vertex vertices_2[] = {
        {{400, 150},{1.0, 0.0, 0.0, 1.0}},
        {{200, 450},{0.0, 1.0, 0.0, 1.0}},
        {{600, 450},{0.0, 0.0, 1.0, 1.0}},
    };
    SDL_RenderGeometry(gSDLRenderer, NULL, vertices_2, 3, NULL, 0); */

    SDL_RenderPresent(gSDLRenderer);
    SDL_Delay(1);

    printf("%s",SDL_GetError());
}