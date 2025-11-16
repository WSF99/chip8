#include "platform.h"
#include "chip8.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>

SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

SDL_Window *debug_win = NULL;
SDL_Renderer *debug_ren = NULL;

static bool debug_initialized = false;
static TTF_Font *debug_font = NULL;

static SDL_Texture *mem_textures[MEM_SIZE] = {0};

static void debug_init_font(void);
static void debug_render_text(SDL_Renderer *ren, int x, int y,
                              const char *text);

int platform_init(const char *title, int x, int y, int w, int h) {
#ifdef USE_SDL
  if (!title)
    return -1;
  return platform_create_window(title, x, y, w, h, &win, &ren);
#else
  (void)title;
  (void)x;
  (void)y;
  (void)w;
  (void)h;
  return 0;
#endif
}

int platform_init_debug(const char *title, int x, int y, int w, int h) {
#ifdef USE_SDL
  if (!title)
    return -1;
  return platform_create_window(title, x, y, w, h, &debug_win, &debug_ren);
#else
  (void)title;
  (void)x;
  (void)y;
  (void)w;
  (void)h;
  return 0;
#endif
}

void platform_handle_input(Input *in) {
#ifdef USE_SDL
  if (!in)
    return;

  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    switch (ev.type) {

    case SDL_QUIT:
      in->exit_signal = 1;
      break;

    case SDL_KEYDOWN:
    case SDL_KEYUP: {
      bool down = (ev.type == SDL_KEYDOWN);
      int key = -1;

      switch (ev.key.keysym.sym) {
      case SDLK_1:
        key = 0x1;
        break;
      case SDLK_2:
        key = 0x2;
        break;
      case SDLK_3:
        key = 0x3;
        break;
      case SDLK_4:
        key = 0xC;
        break;

      case SDLK_q:
        key = 0x4;
        break;
      case SDLK_w:
        key = 0x5;
        break;
      case SDLK_e:
        key = 0x6;
        break;
      case SDLK_r:
        key = 0xD;
        break;

      case SDLK_a:
        key = 0x7;
        break;
      case SDLK_s:
        key = 0x8;
        break;
      case SDLK_d:
        key = 0x9;
        break;
      case SDLK_f:
        key = 0xE;
        break;

      case SDLK_z:
        key = 0xA;
        break;
      case SDLK_x:
        key = 0x0;
        break;
      case SDLK_c:
        key = 0xB;
        break;
      case SDLK_v:
        key = 0xF;
        break;

      case SDLK_ESCAPE:
        in->exit_signal = 1;
        break;
      }

      if (key >= 0)
        input_set_key(in, (uint8_t)key, down);
    } break;

    default:
      break;
    }
  }
#else
  (void)in;
#endif
}

void platform_shutdown(void) {
#ifdef USE_SDL
  for (size_t i = 0; i < MEM_SIZE; i++) {
    if (mem_textures[i]) {
      SDL_DestroyTexture(mem_textures[i]);
      mem_textures[i] = NULL;
    }
  }

  if (debug_font) {
    TTF_CloseFont(debug_font);
    debug_font = NULL;
  }

  if (ren)
    SDL_DestroyRenderer(ren);
  if (win)
    SDL_DestroyWindow(win);
  if (debug_ren)
    SDL_DestroyRenderer(debug_ren);
  if (debug_win)
    SDL_DestroyWindow(debug_win);

  TTF_Quit();
  SDL_Quit();
#endif
}

static void debug_init_font(void) {
#ifdef USE_SDL
  if (debug_initialized)
    return;

  if (TTF_Init() != 0) {
    fprintf(stderr, "[ERROR] TTF_Init failed: %s\n", TTF_GetError());
    return;
  }

  debug_font = TTF_OpenFont("assets/FreeMono.ttf", 18);
  if (!debug_font) {
    fprintf(stderr, "[ERROR] Failed to load font: %s\n", TTF_GetError());
    return;
  }

  debug_initialized = true;
#endif
}

static void debug_render_text(SDL_Renderer *ren, int x, int y,
                              const char *text) {
#ifdef USE_SDL
  if (!ren || !text)
    return;

  SDL_Color white = {255, 255, 255, 255};
  SDL_Surface *surf = TTF_RenderText_Solid(debug_font, text, white);
  if (!surf)
    return;

  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
  SDL_FreeSurface(surf);
  if (!tex)
    return;

  SDL_Rect dst = {x, y, 0, 0};
  SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
  SDL_RenderCopy(ren, tex, NULL, &dst);
  SDL_DestroyTexture(tex);
#endif
}

void debug_render_registers(const Chip8 *c8) {
#ifdef USE_SDL
  if (!debug_ren || !c8)
    return;

  if (!debug_initialized)
    debug_init_font();

  SDL_SetRenderDrawColor(debug_ren, 0, 0, 0, 255);
  SDL_RenderClear(debug_ren);

  const int start_x = 10;
  const int start_y = 10;

  char buf[64];

  for (size_t i = 0; i < 16; i++) {
    size_t col = i / 8;
    size_t row = i % 8;

    snprintf(buf, sizeof(buf), "V%lX: %02X", (unsigned long)i, c8->V[i]);

    debug_render_text(debug_ren, start_x + (int)(col * 100),
                      start_y + (int)(row * 20), buf);
  }

  int y = start_y + 160 + 20;

  snprintf(buf, sizeof(buf), "I : %03X", c8->I);
  debug_render_text(debug_ren, start_x, y, buf);
  y += 20;

  snprintf(buf, sizeof(buf), "PC: %03X", c8->pc);
  debug_render_text(debug_ren, start_x, y, buf);
  y += 20;

  snprintf(buf, sizeof(buf), "SP: %X", c8->sp);
  debug_render_text(debug_ren, start_x, y, buf);
  y += 20;

  snprintf(buf, sizeof(buf), "DT: %02X", c8->timers.dt);
  debug_render_text(debug_ren, start_x, y, buf);
  y += 20;

  snprintf(buf, sizeof(buf), "ST: %02X", c8->timers.st);
  debug_render_text(debug_ren, start_x, y, buf);

  SDL_RenderPresent(debug_ren);
#endif
}

void platform_update(const Chip8 *c8) {
#ifdef USE_SDL
  if (!c8)
    return;

  for (size_t y = 0; y < DISPLAY_HEIGHT; y++) {
    for (size_t x = 0; x < DISPLAY_WIDTH; x++) {

      uint8_t pixel = c8->disp.pixels[y * DISPLAY_WIDTH + x];
      SDL_SetRenderDrawColor(ren, pixel ? 255 : 0, pixel ? 255 : 0,
                             pixel ? 255 : 0, 255);

      SDL_Rect rect = {(int)(x * 10), (int)(y * 10), 10, 10};

      SDL_RenderFillRect(ren, &rect);
    }
  }

  SDL_RenderPresent(ren);

  if (debug_ren)
    debug_render_registers(c8);
#endif
}

int platform_create_window(const char *title, int x, int y, int w, int h,
                           SDL_Window **out_win, SDL_Renderer **out_ren) {
#ifdef USE_SDL
  if (!title || !out_win || !out_ren)
    return -1;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    return -2;

  *out_win = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
  if (!*out_win)
    return -3;

  *out_ren = SDL_CreateRenderer(*out_win, -1, SDL_RENDERER_ACCELERATED);
  if (!*out_ren)
    return -4;

  SDL_SetRenderDrawColor(*out_ren, 0, 0, 0, 255);
  SDL_RenderClear(*out_ren);
  SDL_RenderPresent(*out_ren);
#endif

  return 0;
}
