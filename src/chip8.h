#ifndef CHIP8_H
#define CHIP8_H

#define NUM_REGS 16
#define STACK_SIZE 16

#include <stdint.h>
#include "memory.h"
#include "display.h"
#include "input.h"
#include "timers.h"

typedef struct Chip8 {
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

void chip8_init(Chip8 *c8);
void chip8_reset(Chip8 *c8);
void chip8_cycle(Chip8 *c8);

#endif // CHIP8_H
