#include "display.h"

void display_init(Display *disp) {
  for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
    disp->pixels[i] = 0;
  }
}

void display_clear(Display *disp) { display_init(disp); }

int display_draw_sprite(Display *disp, Memory *m, uint16_t I, uint8_t x,
                        uint8_t y, uint8_t n) {
  int collision = 0;
  for (int row = 0; row < n; row++) {
    uint8_t sprite_byte = m->memory[I + row];
    for (int col = 0; col < 8; col++) {
      if ((sprite_byte & (0x80 >> col)) != 0) {
        int px = (x + col) % DISPLAY_WIDTH;
        int py = (y + row) % DISPLAY_HEIGHT;
        int index = py * DISPLAY_WIDTH + px;
        if (disp->pixels[index] == 1) {
          collision = 1;
        }
        disp->pixels[index] ^= 1;
      }
    }
  }
  return collision;
}
