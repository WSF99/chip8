#include "timers.h"

void timers_init(Timers *timers) {
    timers->delay_timer = 0;
    timers->sound_timer = 0;
}

void timers_reset(Timers *timers) {
    timers_init(timers);
}

void timers_update(Timers *timers) {
    if (timers->delay_timer > 0) {
        timers->delay_timer--;
    }
    if (timers->sound_timer > 0) {
        timers->sound_timer--;
    }
}
