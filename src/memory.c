#include "memory.h"
#include <string.h>
#include <stdio.h>

void memory_init(Memory *m) {
    memset(m->memory, 0, sizeof(m->memory));
    memcpy(&m->memory[FONTSET_ADDR], chip8_fontset, sizeof(chip8_fontset));
}

uint8_t memory_read(Memory *m, uint16_t addr) {
    if (addr >= MEM_SIZE) return 0;
    return m->memory[addr];
}

void memory_write(Memory *m, uint16_t addr, uint8_t val) {
    if (addr >= MEM_SIZE) return;
    m->memory[addr] = val;
}

int memory_load_rom(Memory *m, const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size <= 0 || START_ADDR + size > MEM_SIZE) {
        fclose(file);
        return -2;
    }

    fread(&m->memory[START_ADDR], 1, size, file);
    fclose(file);
    return 0;
}
