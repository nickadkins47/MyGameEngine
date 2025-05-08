
#include <SDL3/SDL_main.h>

#include "test.hh"

#include <print>

char const* const engName = "MyGameEngine";

int main(int argc, char** argv) {
    std::print("{}: Starting Up\n", engName);
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* gSDLWindow = SDL_CreateWindow(engName, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);

    while(update(gSDLWindow, gSDLRenderer));

    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();

    std::print("{}: Shutting Down\n", engName);
    return 0;
}
