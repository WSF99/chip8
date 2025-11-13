#ifndef PLATFORM_H
#define PLATFORM_H

#include "chip8.h"

int platform_init(Chip8 *chip8, const char *rom_path);
void platform_cleanup();

#endif // PLATFORM_H
