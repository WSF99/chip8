#include "chip8.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint16_t fetch_opcode(Chip8 *chip8) {
  uint8_t hi = memory_read(&chip8->mem, chip8->pc);
  uint8_t lo = memory_read(&chip8->mem, chip8->pc + 1);
  return (hi << 8) | lo;
}

void chip8_cycle(Chip8 *c8) {

  uint16_t opcode = fetch_opcode(c8);
  c8->pc += 2;
  uint16_t op0 = (opcode & 0xF000) >> 12;
  uint16_t nnn = opcode & 0x0FFF;
  uint8_t n = opcode & 0x000F;
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  uint8_t kk = opcode & 0x00FF;
  switch (op0) {
  case 0x0:
    switch (kk) {
    case 0xE0: // CLS
      memset(&c8->disp.pixels, 0, sizeof(c8->disp.pixels));
      break;
    case 0xEE: // RET
      c8->sp--;
      c8->pc = c8->stack[c8->sp];
      break;
    }
    break;
  case 0x1: // JP addr
    c8->pc = nnn;
    break;
  case 0x2: // CALL addr
    c8->stack[c8->sp] = c8->pc;
    c8->sp++;
    c8->pc = nnn;
    break;
  case 0x3: // SE Vx, byte
    if (c8->V[x] == kk) {
      c8->pc += 2;
    }
    break;
  case 0x4: // SNE Vx, byte
    if (c8->V[x] != kk) {
      c8->pc += 2;
    }
    break;
  case 0x5: // SE Vx, Vy
    if (c8->V[x] == c8->V[y]) {
      c8->pc += 2;
    }
    break;
  case 0x6: // LD Vx, byte
    c8->V[x] = kk;
    break;
  case 0x7: // ADD Vx, byte
    c8->V[x] += kk;
    break;
  case 0x8:
    switch (n) {
    case 0x0: // LD Vx, Vy
      c8->V[x] = c8->V[y];
      break;
    case 0x1: // OR Vx, Vy
      c8->V[x] |= c8->V[y];
      break;
    case 0x2: // AND Vx, Vy
      c8->V[x] &= c8->V[y];
      break;
    case 0x3: // XOR Vx, Vy
      c8->V[x] ^= c8->V[y];
      break;
    case 0x4: // ADD Vx, Vy
      uint16_t sum = c8->V[x] + c8->V[y];
      c8->V[0xF] = (sum > 0xFF) ? 1 : 0;
      c8->V[x] = sum & 0xFF;
      break;
    case 0x5: // SUB Vx, Vy
      c8->V[0xF] = (c8->V[x] > c8->V[y]) ? 1 : 0;
      c8->V[x] -= c8->V[y];
      break;
    case 0x6: // SHR Vx {, Vy}
      c8->V[0xF] = c8->V[x] & 0x1;
      c8->V[x] >>= 1;
      break;
    case 0x7: // SUBN Vx, Vy
      c8->V[0xF] = (c8->V[y] > c8->V[x]) ? 1 : 0;
      c8->V[x] = c8->V[y] - c8->V[x];
      break;
    case 0xE: // SHL Vx {, Vy}
      c8->V[0xF] = (c8->V[x] & 0x80) >> 7;
      c8->V[x] <<= 1;
      break;
    }
    break;
  case 0x9: // SNE Vx, Vy
    if (c8->V[x] != c8->V[y]) {
      c8->pc += 2;
    }
    break;
  case 0xA: // LD I, addr
    c8->I = nnn;
    break;
  case 0xB: // JP V0, addr
    c8->pc = nnn + c8->V[0];
    break;
  case 0xC: // RND Vx, byte
    c8->V[x] = (rand() % 256) & kk;
    break;
  case 0xD: { // DRW Vx, Vy, nibble
    int col =
        display_draw_sprite(&c8->disp, &c8->mem, c8->I, c8->V[x], c8->V[y], n);
    c8->V[0xF] = col ? 1 : 0;
    break;
  }
  case 0xE:
    switch (kk) {
    case 0x9E: // SKP Vx
      if (c8->input.keys[c8->V[x]] != 0) {
        c8->pc += 2;
      }
      break;
    case 0xA1: // SKNP Vx
      if (c8->input.keys[c8->V[x]] == 0) {
        c8->pc += 2;
      }
      break;
    }
    break;
  case 0xF:
    switch (kk) {
    case 0x07: // LD Vx, DT
      c8->V[x] = c8->timers.delay_timer;
      break;
    case 0x0A: // LD Vx, K
      input_wait_for_key(&c8->input, x);
      break;
    case 0x15: // LD DT, Vx
      c8->timers.delay_timer = c8->V[x];
      break;
    case 0x18: // LD ST, Vx
      c8->timers.sound_timer = c8->V[x];
      break;
    case 0x1E: // ADD I, Vx
      c8->I += c8->V[x];
      break;
    case 0x29: // LD F, Vx
      c8->I = FONTSET_ADDR + (c8->V[x] * 5);
      break;
    case 0x33: // LD B, Vx
      c8->mem.memory[c8->I] = c8->V[x] / 100;
      c8->mem.memory[c8->I + 1] = (c8->V[x] / 10) % 10;
      c8->mem.memory[c8->I + 2] = c8->V[x] % 10;
      break;
    case 0x55: // LD [I], Vx
      for (int i = 0; i <= x; i++) {
        c8->mem.memory[c8->I + i] = c8->V[i];
      }
      break;
    case 0x65: // LD Vx, [I]
      for (int i = 0; i <= x; i++) {
        c8->V[i] = c8->mem.memory[c8->I + i];
      }
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
  for (int i = 0; i < NUM_REGS; i++) {
    c8->V[i] = 0;
  }
  for (int i = 0; i < STACK_SIZE; i++) {
    c8->stack[i] = 0;
  }
}

void chip8_init(Chip8 *c8) { chip8_reset(c8); }
