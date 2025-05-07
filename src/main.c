
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "test.h"

int* gFrameBuffer;
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
static int gDone;
const int WINDOW_WIDTH = 1920 / 2;
const int WINDOW_HEIGHT = 1080 / 2;

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

    char* pix;
    int pitch;
  
    SDL_LockTexture(gSDLTexture, NULL, (void**)&pix, &pitch);
    for (int i = 0, sp = 0, dp = 0; i < WINDOW_HEIGHT; i++, dp += WINDOW_WIDTH, sp += pitch)
        memcpy(pix + sp, gFrameBuffer + dp, WINDOW_WIDTH * 4);

    SDL_UnlockTexture(gSDLTexture);
    SDL_RenderTexture(gSDLRenderer, gSDLTexture, NULL, NULL);
    SDL_RenderPresent(gSDLRenderer);
    SDL_Delay(1);
    return true;
}

void putpixel(int x, int y, int color) {
    if (x < 0 || x >= WINDOW_HEIGHT || y < 0 || y >= WINDOW_WIDTH) return;
    gFrameBuffer[x * WINDOW_WIDTH + y] = color;
}

void render(Uint64 aTicks) {
    for (int x = 0; x < WINDOW_HEIGHT; x++) {
        for (int y = 0; y < WINDOW_WIDTH; y++) {
            putpixel(x, y, 0xffff0000);
        }
    }
}

#define MGEInit(x) if (!(x)) return -1

int main(int argc, char** argv) {
    
    MGEInit(SDL_Init(SDL_INIT_VIDEO));
    MGEInit(gFrameBuffer = (int*)malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT));
    MGEInit(gSDLWindow = SDL_CreateWindow("MyGameEngine", WINDOW_WIDTH, WINDOW_HEIGHT, 0));
    MGEInit(gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL));
    MGEInit(gSDLTexture = SDL_CreateTexture(
        gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT
    ));

    gDone = 0;
    while (!gDone) {
        if (!update()) {
            gDone = 1;
        } else {
            render(SDL_GetTicks());
        }
    }

    SDL_DestroyTexture(gSDLTexture);
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();
    free(gFrameBuffer);

    test();

    return 0;
}