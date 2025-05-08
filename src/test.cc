
#include "test.hh"

bool update(SDL_Window* gSDLWindow, SDL_Renderer* gSDLRenderer) {
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

    SDL_SetRenderDrawColor(gSDLRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gSDLRenderer);
    //SDL_SetRenderDrawColor(gSDLRenderer, 1, 0, 0, 255);

    SDL_Vertex vertices_1[] = {
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

    /* //basic triangle
    SDL_Vertex vertices_2[] = {
        {{400, 150},{1.0, 0.0, 0.0, 1.0}},
        {{200, 450},{0.0, 1.0, 0.0, 1.0}},
        {{600, 450},{0.0, 0.0, 1.0, 1.0}},
    };
    SDL_RenderGeometry(gSDLRenderer, NULL, vertices_2, 3, NULL, 0); */

    SDL_RenderPresent(gSDLRenderer);
    SDL_Delay(1);

    printf("%s",SDL_GetError());
    return true;
}
