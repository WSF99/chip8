#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>

enum { NUM_KEYS = 16 };

typedef struct Input {
  uint8_t keys[NUM_KEYS];
  int8_t waiting_for_key;
  bool exit_signal;
} Input;

void input_init(Input *in);
void input_set_key(Input *in, uint8_t key, uint8_t pressed);
void input_wait_for_key(Input *in, uint8_t register_idx);

#endif // INPUT_H
