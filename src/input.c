#include "input.h"

void input_init(Input *in) {
    for (int i = 0; i < 16; i++) {
        in->keys[i] = 0;
    }
}

void input_set_key(Input *in, int key, int pressed) {
    if (key >= 0 && key < 16) {
        in->keys[key] = pressed;
    }
}

int input_any_pressed(Input *in) {
    for (int i = 0; i < 16; i++) {
        if (in->keys[i]) {
            return 1;
        }
    }
    return 0;
}

void input_wait_for_key(Input *in) {
    in->waiting_for_key = -2;
}
