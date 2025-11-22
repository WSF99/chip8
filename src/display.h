#ifndef DISPLAY_H
#define DISPLAY_H

#include "memory.h"
#include <stddef.h>
#include <stdint.h>

enum {
  DISPLAY_WIDTH = 64,
  DISPLAY_HEIGHT = 32,
  DISPLAY_SIZE = DISPLAY_WIDTH * DISPLAY_HEIGHT
};

typedef struct {
  uint8_t pixels[DISPLAY_SIZE];
} Display;

void display_init(Display *disp);
void display_clear(Display *disp);
uint8_t display_draw_sprite(Display *disp, Memory *m, uint16_t I, uint8_t x,
                            uint8_t y, uint8_t n);

#endif // DISPLAY_H
