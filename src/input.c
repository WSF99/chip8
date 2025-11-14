#include "input.h"
#include <stdio.h>

void input_init(Input *in) {
    for (int i = 0; i < 16; i++) {
        in->keys[i] = 0;
    }
    in->waiting_for_key = -1;
    in->exit_signal = 0;
}


void input_set_key(Input *in, uint8_t key, uint8_t pressed) {
    if (key < 16) {
        in->keys[key] = pressed;
    }
}

void input_wait_for_key(Input *in, uint8_t register_idx) {
    in->waiting_for_key = register_idx;
}
