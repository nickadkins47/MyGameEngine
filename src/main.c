
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "test.h"

/* int* gFrameBuffer;
int *gTempBuffer; */
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
/* SDL_Texture* gSDLTexture; */
static int gDone;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

SDL_Vertex vertices[] = {
    {{400, 150},{1.0, 0.0, 0.0, 1.0}},
    {{200, 450},{0.0, 1.0, 0.0, 1.0}},
    {{600, 450},{0.0, 0.0, 1.0, 1.0}},
};

bool update() {
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            return false;
        }
        if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE) {
            return false;
        }
    }

    //char* pix;
    //int pitch;
  
    /* SDL_LockTexture(gSDLTexture, NULL, (void**)&pix, &pitch);
    for (int i = 0, sp = 0, dp = 0; i < WINDOW_HEIGHT; i++, dp += WINDOW_WIDTH, sp += pitch)
        memcpy(pix + sp, gFrameBuffer + dp, WINDOW_WIDTH * 4);

    SDL_UnlockTexture(gSDLTexture);
    SDL_RenderTexture(gSDLRenderer, gSDLTexture, NULL, NULL); */
    
    SDL_RenderPresent(gSDLRenderer);
    SDL_Delay(1);
    return true;
}

void render() {
    SDL_SetRenderDrawColor(gSDLRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gSDLRenderer);

    SDL_RenderGeometry(gSDLRenderer, NULL, vertices, 3, NULL, 0);
}

#define MGEInit(x) if (!(x)) return -1

int main(int argc, char** argv) {
    
    MGEInit(SDL_Init(SDL_INIT_VIDEO));
    /* MGEInit(gFrameBuffer = (int*)malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT));
    MGEInit(gTempBuffer = (int*)malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT)); */
    MGEInit(gSDLWindow = SDL_CreateWindow("MyGameEngine", WINDOW_WIDTH, WINDOW_HEIGHT, 0));
    MGEInit(gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL));
    /* MGEInit(gSDLTexture = SDL_CreateTexture(
        gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT
    )); */

    /* for (int c = 0; c < WINDOW_HEIGHT * WINDOW_WIDTH; c++) {
        gFrameBuffer[c] = 0xff000000;
        gTempBuffer[c] = 0xff000000;
    } */

    gDone = 0;
    while (!gDone) {
        if (!update()) {
            gDone = 1;
        } else {
            render();
        }
    }

    /* SDL_DestroyTexture(gSDLTexture); */
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();
    /* free(gFrameBuffer);
    free(gTempBuffer); */

    test();

    return 0;
}