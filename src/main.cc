
#include <SDL3/SDL_main.h>

#include "EngineCore.hh"
#include "Obj/Quad.hh"

int main(int argc, char** argv) {
    MGE::EngineCore engine;
    
    engine.add_obj(MGE::ObjQuad({0.0f, 0.0f, 0.0f}, MGE::QuadOrientation::XY));

    engine.add_objs({
        MGE::ObjQuad({0.0f, 0.0f, 0.0f}, MGE::QuadOrientation::XZ),
        MGE::ObjQuad({0.0f, 0.0f, 0.0f}, MGE::QuadOrientation::YZ),
        MGE::ObjQuad({0.0f, 0.0f, 1.0f}, MGE::QuadOrientation::XY),
        MGE::ObjQuad({0.0f, 1.0f, 0.0f}, MGE::QuadOrientation::XZ),
        MGE::ObjQuad({1.0f, 0.0f, 0.0f}, MGE::QuadOrientation::YZ),
    });
    
    while(!engine.shutdown) engine.update();

    return 0;
}
