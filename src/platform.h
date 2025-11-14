#ifndef PLATFORM_H
#define PLATFORM_H
#include "input.h"
#include "display.h"

int platform_init(const char *title, int x, int y, int w, int h);
void platform_update(Display *disp);
void platform_handle_input(Input *in);
void platform_shutdown();
#endif // PLATFORM_H
