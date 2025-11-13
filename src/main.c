#include <stdio.h>
#include <platform.h>

int main (int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <ROM file>\n", argv[0]);
        return 1;
    }
    
    Chip8 c8;
    return platform_init(&c8, argv[1]);
}
