#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "platform.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <ROM file>\n", argv[0]);
        return 1;
    }

    Chip8 c8;
    chip8_init(&c8);

    if (memory_load_rom(&c8.mem, argv[1]) != 0) {
        fprintf(stderr, "Failed to load ROM: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (platform_init(
            "Chip8 Emulator",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640, 320) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL platform\n");
        return 1;
    }

    uint32_t last_cpu_tick   = SDL_GetTicks();
    uint32_t last_timer_tick = SDL_GetTicks();

    while (!c8.input.exit_signal) {
        uint32_t now = SDL_GetTicks();
        platform_handle_input(&c8.input);
        if (now - last_cpu_tick >= CYCLE_DELAY_MS) {
            chip8_cycle(&c8);
            platform_update(&c8.disp);
            last_cpu_tick = now;
        }

        if (c8.input.waiting_for_key != -1) {
            for (uint8_t k = 0; k < 16; k++) {
                if (c8.input.keys[k]) {
                    c8.V[c8.input.waiting_for_key] = k;
                    c8.input.waiting_for_key = -1;
                    break;
                }
            }
        }

        if (now - last_timer_tick >= (1000 / TIMER_HZ)) {
            timers_update(&c8.timers);
            last_timer_tick = now;
        }
    }

    platform_shutdown();
    return 0;
}
