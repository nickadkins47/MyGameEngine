
#include "EngineCore.hh"

namespace MGE {

    EngineCore::EngineCore() {
        print("{}: Starting Up\n", engName);
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(engName.data(), windowWidth, windowHeight, 0);
        renderer = SDL_CreateRenderer(window, NULL);

        cam.kbdPtr = &kbd;
        cam.setProjMat(fov, windowWidth, windowHeight, zNear, zFar);
    }

    EngineCore::~EngineCore() {
        print("{}: Shutting Down\n", engName);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (Obj const& o : obj_list) {
            Vector<Triangle<4>> newMesh {};

            for (Triangle<3> const& tri : o.mesh) {
                Triangle<4> newTri {Vec<4>{0,0,0,0}, Vec<4>{0,0,0,0}, Vec<4>{0,0,0,0}};
                bool withinClippingVolume = true;
                for (Int i = 0; i < tri.size(); i++) {
                    newTri[i] = vpMat * Vec<4>{ tri[i][0], tri[i][1], tri[i][2], 1.0f };
                    if (newTri[i][2] < zNear) {
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
                    {(newTri[0][0] + 1.0f) * (windowWidth/2), (newTri[0][1] + 1.0f) * (windowHeight/2)},
                    {(newTri[1][0] + 1.0f) * (windowWidth/2), (newTri[1][1] + 1.0f) * (windowHeight/2)},
                    {(newTri[2][0] + 1.0f) * (windowWidth/2), (newTri[2][1] + 1.0f) * (windowHeight/2)},
                    {(newTri[0][0] + 1.0f) * (windowWidth/2), (newTri[0][1] + 1.0f) * (windowHeight/2)},
                };
                SDL_RenderLines(renderer, fpoints, 4);

                SDL_Vertex vertices[] = {
                    {fpoints[0], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                    {fpoints[1], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                    {fpoints[2], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                    {fpoints[3], {0.5f, 0.5f, 0.5f, 1.0f}, {0,0}},
                };
                Int indices[] = { 0,1,2, 2,3,0 };
                SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1);

        print("{}", SDL_GetError());

    }

}