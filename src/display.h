#ifndef DISPLAY_H
#define DISPLAY_H

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

#include <stdint.h>
#include "memory.h"

typedef struct Display {
    uint8_t pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];
} Display;

void display_init(Display *disp);
void display_clear(Display *disp);
int display_draw_sprite(Display *disp, Memory *m, uint16_t I, uint8_t x, uint8_t y, uint8_t n);

#endif // DISPLAY_H
