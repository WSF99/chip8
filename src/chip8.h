#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define START_ADDR 0x200
#define FONTSET_ADDR 0x50
#define NUM_REGS 16
#define STACK_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define MEM_SIZE 4096
#define TIMER_HZ 60

#include "memory.h"
#include "display.h"
#include "input.h"

typedef struct {
    uint8_t memory[MEM_SIZE];
} Memory;

typedef struct {
    uint8_t pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];
} Display;

typedef struct {
    uint8_t keys[16];
    int waiting_for_key;
} Input;

typedef struct {
    uint8_t delay_timer;
    uint8_t sound_timer;
} Timers;

typedef struct {
    Memory mem;
    Display disp;
    Input input;
    Timers timers;
    uint8_t V[NUM_REGS];
    uint16_t I;
    uint16_t pc;
    uint8_t sp;
    uint16_t stack[STACK_SIZE];
} Chip8;

extern const uint8_t chip8_fontset[80];

void chip8_init(Chip8 *chip8);
void chip8_reset(Chip8 *chip8);

#endif // CHIP8_H
