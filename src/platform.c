#include <SDL2/SDL.h>
#include "platform.h"

SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

int platform_init(const char *title, int x, int y, int w, int h) {
#ifdef USE_SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }
    win = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return -2;
    }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -3;
    }
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
#endif

    return 0;
}

void platform_handle_input(Input *in) {
#ifdef USE_SDL
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
            in->exit_signal = 1;
        }
        if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP) {
            int down = (ev.type == SDL_KEYDOWN);
            int key = -1;
            switch (ev.key.keysym.sym) {
                case SDLK_1: key = 0x1; break;
                case SDLK_2: key = 0x2; break;
                case SDLK_3: key = 0x3; break;
                case SDLK_4: key = 0xC; break;
                case SDLK_q: key = 0x4; break;
                case SDLK_w: key = 0x5; break;
                case SDLK_e: key = 0x6; break;
                case SDLK_r: key = 0xD; break;
                case SDLK_a: key = 0x7; break;
                case SDLK_s: key = 0x8; break;
                case SDLK_d: key = 0x9; break;
                case SDLK_f: key = 0xE; break;
                case SDLK_z: key = 0xA; break;
                case SDLK_x: key = 0x0; break;
                case SDLK_c: key = 0xB; break;
                case SDLK_v: key = 0xF; break;
                case SDLK_ESCAPE:
                    in->exit_signal = 1;
                    break;
            }
            if (key >= 0) {
                input_set_key(in, key, down);
            }
        }
    }
#endif
}

void platform_shutdown() {
#ifdef USE_SDL
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
#endif
}

void platform_update(Display *disp) {
#ifdef USE_SDL
    SDL_UpdateTexture(NULL, NULL, NULL, 0);
    SDL_RenderClear(ren);
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            if (disp->pixels[y * DISPLAY_WIDTH + x]) {
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            }
            SDL_Rect rect = { x * 10, y * 10, 10, 10 };
            SDL_RenderFillRect(ren, &rect);
        }
    }
    SDL_RenderPresent(ren);
#endif
}