#include "input.h"
#include <string.h>

void input_init(Input *in) {
  memset(in->keys, 0, sizeof in->keys);
  in->waiting_for_key = -1;
  in->exit_signal = 0;
}

void input_set_key(Input *in, uint8_t key, uint8_t pressed) {
  if (key < 16)
    in->keys[key] = pressed;
}

void input_wait_for_key(Input *in, uint8_t register_idx) {
  in->waiting_for_key = register_idx;
}
