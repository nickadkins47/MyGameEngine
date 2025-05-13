
#include "update.hh"

bool update(SDL_Window* window, SDL_Renderer* renderer) {

    static std::array<bool,6> movements {false};
    static glm::vec3 camPos {0.0f, 0.0f, 0.0f};
    static glm::vec3 camAng {0.0f, 0.0f, 0.0f};

    if (SDL_Event e; SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT:
                return false;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                SDL_SetWindowRelativeMouseMode(window, true); break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                SDL_SetWindowRelativeMouseMode(window, false); break;
            case SDL_EVENT_MOUSE_MOTION: {
                if (SDL_GetWindowRelativeMouseMode(window)) {
                    camAng[0] += e.motion.yrel * 0.125f;
                    camAng[1] += e.motion.xrel * 0.125f;
                    if (camAng[0] < -89.0f) {
                        camAng[0] = -89.0f;
                    } else if (camAng[0] > 89.0f) {
                        camAng[0] = 89.0f;
                    }
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

                    case SDLK_Z: {
                        std::print("({}, {}, {})\n", camPos[0], camPos[1], camPos[2]);
                        break;
                    }

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
    
    glm::mat4 vpMat = camUpdate(camPos, camAng, movements);
    render(window, renderer, vpMat);
    return true;
}
