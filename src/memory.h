#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

enum { START_ADDR = 0x200, MEM_SIZE = 4096, FONTSET_ADDR = 0x50 };

typedef struct {
  uint8_t memory[MEM_SIZE];
} Memory;

void memory_init(Memory *m);
uint8_t memory_read(const Memory *m, uint16_t addr);
void memory_write(Memory *m, uint16_t addr, uint8_t val);
int memory_load_rom(Memory *m, const char *path);

extern const uint8_t chip8_fontset[80];

#endif // MEMORY_H
