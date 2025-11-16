#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "display.h"
#include "input.h"
#include "memory.h"
#include "timers.h"

enum { CHIP8_NUM_REGS = 16, CHIP8_STACK_LEN = 16 };

typedef struct {
  Memory mem;
  Display disp;
  Input input;
  Timers timers;
  uint8_t V[CHIP8_NUM_REGS];
  uint16_t I;
  uint16_t pc;
  uint8_t sp;
  uint16_t stack[CHIP8_STACK_LEN];
} Chip8;

void chip8_init(Chip8 *c8);
void chip8_reset(Chip8 *c8);
void chip8_cycle(Chip8 *c8);

#endif // CHIP8_H
