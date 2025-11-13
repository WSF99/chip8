#ifndef DISPLAY_H
#define DISPLAY_H
#include <chip8.h>

void display_init(Display *disp);
void display_clear(Display *disp);
int display_draw_sprite(Display *d, Memory *m, uint16_t I, uint8_t x, uint8_t y, uint8_t n);

#endif // DISPLAY_H
