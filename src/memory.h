#ifndef MEMORY_H
#define MEMORY_H
#define MEM_SIZE 4096
#include <stdint.h>
#include <chip8.h>

void memory_init(Memory *m);
uint8_t memory_read(Memory *m, uint16_t addr);
void memory_write(Memory *m, uint16_t addr, uint8_t val);
int memory_load_rom(Memory *m, const char *path);

#endif // MEMORY_H
