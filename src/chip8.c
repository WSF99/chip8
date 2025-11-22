#include "chip8.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint16_t fetch_opcode(Chip8 *chip8) {
  const uint8_t hi = memory_read(&chip8->mem, chip8->pc);
  const uint8_t lo = memory_read(&chip8->mem, chip8->pc + 1);
  return (uint16_t)((hi << 8) | lo);
}

void chip8_cycle(Chip8 *c8) {
  const uint16_t opcode = fetch_opcode(c8);
  c8->pc += 2;

  const uint8_t op0 = (opcode & 0xF000u) >> 12;
  const uint16_t nnn = opcode & 0x0FFFu;
  const uint8_t n = opcode & 0x000Fu;
  const uint8_t x = (opcode & 0x0F00u) >> 8;
  const uint8_t y = (opcode & 0x00F0u) >> 4;
  const uint8_t kk = opcode & 0x00FFu;

  switch (op0) {
  case 0x0:
    switch (kk) {
    case 0xE0:
      memset(c8->disp.pixels, 0, sizeof c8->disp.pixels);
      break;
    case 0xEE:
      if (c8->sp > 0) {
        c8->sp--;
        c8->pc = c8->stack[c8->sp];
      }
      break;
    }
    break;

  case 0x1:
    c8->pc = nnn;
    break;

  case 0x2:
    if (c8->sp < CHIP8_STACK_LEN) {
      c8->stack[c8->sp] = c8->pc;
      c8->sp++;
      c8->pc = nnn;
    }
    break;

  case 0x3:
    if (c8->V[x] == kk)
      c8->pc += 2;
    break;

  case 0x4:
    if (c8->V[x] != kk)
      c8->pc += 2;
    break;

  case 0x5:
    if (c8->V[x] == c8->V[y])
      c8->pc += 2;
    break;

  case 0x6:
    c8->V[x] = kk;
    break;

  case 0x7:
    c8->V[x] = (uint8_t)(c8->V[x] + kk);
    break;

  case 0x8:
    switch (n) {
    case 0x0:
      c8->V[x] = c8->V[y];
      break;
    case 0x1:
      c8->V[x] |= c8->V[y];
      break;
    case 0x2:
      c8->V[x] &= c8->V[y];
      break;
    case 0x3:
      c8->V[x] ^= c8->V[y];
      break;
    case 0x4: {
      const uint16_t sum = (uint16_t)c8->V[x] + c8->V[y];
      c8->V[0xF] = sum > 0xFFu;
      c8->V[x] = (uint8_t)(sum & 0xFFu);
      break;
    }
    case 0x5:
      c8->V[0xF] = c8->V[x] > c8->V[y];
      c8->V[x] = (uint8_t)(c8->V[x] - c8->V[y]);
      break;
    case 0x6:
      c8->V[0xF] = c8->V[x] & 1u;
      c8->V[x] >>= 1;
      break;
    case 0x7:
      c8->V[0xF] = c8->V[y] > c8->V[x];
      c8->V[x] = (uint8_t)(c8->V[y] - c8->V[x]);
      break;
    case 0xE:
      c8->V[0xF] = (c8->V[x] >> 7) & 1u;
      c8->V[x] <<= 1;
      break;
    }
    break;

  case 0x9:
    if (c8->V[x] != c8->V[y])
      c8->pc += 2;
    break;

  case 0xA:
    c8->I = nnn;
    break;

  case 0xB:
    c8->pc = nnn + c8->V[0];
    break;

  case 0xC:
    c8->V[x] = (uint8_t)((rand() & 0xFF) & kk);
    break;

  case 0xD: {
    const uint8_t col =
        display_draw_sprite(&c8->disp, &c8->mem, c8->I, c8->V[x], c8->V[y], n);
    c8->V[0xF] = col ? 1u : 0u;
    break;
  }

  case 0xE:
    switch (kk) {
    case 0x9E:
      if (c8->input.keys[c8->V[x]])
        c8->pc += 2;
      break;
    case 0xA1:
      if (!c8->input.keys[c8->V[x]])
        c8->pc += 2;
      break;
    }
    break;

  case 0xF:
    switch (kk) {
    case 0x07:
      c8->V[x] = c8->timers.dt;
      break;
    case 0x0A:
      input_wait_for_key(&c8->input, x);
      break;
    case 0x15:
      c8->timers.dt = c8->V[x];
      break;
    case 0x18:
      c8->timers.st = c8->V[x];
      break;
    case 0x1E:
      c8->I = (uint16_t)(c8->I + c8->V[x]);
      break;
    case 0x29:
      c8->I = (uint16_t)(FONTSET_ADDR + c8->V[x] * 5u);
      break;
    case 0x33:
      c8->mem.memory[c8->I] = (uint8_t)(c8->V[x] / 100);
      c8->mem.memory[c8->I + 1] = (uint8_t)((c8->V[x] / 10) % 10);
      c8->mem.memory[c8->I + 2] = (uint8_t)(c8->V[x] % 10);
      break;
    case 0x55:
      for (size_t i = 0; i <= x; i++)
        c8->mem.memory[c8->I + i] = c8->V[i];
      break;
    case 0x65:
      for (size_t i = 0; i <= x; i++)
        c8->V[i] = c8->mem.memory[c8->I + i];
      break;
    }
    break;
  }
}

void chip8_reset(Chip8 *c8) {
  memory_init(&c8->mem);
  display_init(&c8->disp);
  input_init(&c8->input);

  c8->I = 0;
  c8->pc = START_ADDR;
  c8->sp = 0;

  for (size_t i = 0; i < CHIP8_NUM_REGS; i++)
    c8->V[i] = 0;

  for (size_t i = 0; i < CHIP8_STACK_LEN; i++)
    c8->stack[i] = 0;
}

void chip8_init(Chip8 *c8) { chip8_reset(c8); }
