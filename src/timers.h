#ifndef TIMERS_H
#define TIMERS_H
#include <stdint.h>
#define TIMER_HZ 60
#define CYCLE_DELAY_MS 2

typedef struct Timers {
  uint8_t dt;
  uint8_t st;
} Timers;

void timers_update(Timers *timers);
void timers_init(Timers *timers);
void timers_reset(Timers *timers);
#endif // TIMERS_H
