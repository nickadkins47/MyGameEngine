
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

        void update();

        void add_obj(Obj const& o);
        void add_objs(Vector<Obj> const& o_vec);
        void render();

    //protected:
        bool shutdown = false;

        Camera cam;
        Keyboard kbd;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        Vector<Obj> obj_list {};

        Int static inline const windowWidth = 1200;
        Int static inline const windowHeight = 900;
        Float static inline const fov = 90.0f;
        Float static inline const zNear = 0.1f;
        Float static inline const zFar = 100.0f;

        StringView static inline const engName {"MyGameEngine"};

    };

}