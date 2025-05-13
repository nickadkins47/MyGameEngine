
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <SDL3/SDL_main.h>

#include "update.hh"

char const* const engName = "MyGameEngine";

int main(int argc, char** argv) {
    print("{}: Starting Up\n", engName);

    //print("{}\n", glm::pi<float>());

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(engName, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    renderQuad({0.0f, 0.0f, 0.0f}, QuadOrientation::XY);
    renderQuad({0.0f, 0.0f, 0.0f}, QuadOrientation::XZ);
    renderQuad({0.0f, 0.0f, 0.0f}, QuadOrientation::YZ);

    renderQuad({0.0f, 0.0f, 1.0f}, QuadOrientation::XY);
    renderQuad({0.0f, 1.0f, 0.0f}, QuadOrientation::XZ);
    renderQuad({1.0f, 0.0f, 0.0f}, QuadOrientation::YZ);
    
    while(update(window, renderer));

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    print("{}: Shutting Down\n", engName);
    return 0;
}
