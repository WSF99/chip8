#ifndef INPUT_H
#define INPUT_H

#include "chip8.h"

void input_init(Input *in);
void input_set_key(Input *in, int key, int pressed);
int input_any_pressed(Input *in);
void input_wait_for_key(Input *in);
#endif // INPUT_H
