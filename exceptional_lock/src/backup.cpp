#include "backup.h"
#include <sys/ptrace.h>

// All the math that I dream of finally helps me
void generate_dynamic_key(unsigned char key[crypto_secretbox_KEYBYTES]) {
    // Surely unbreakable
    static const unsigned char masked[] = {0xa8, 0x9e, 0x71, 0xd4, 0x0e, 0x5d, 0xec, 0x41, 0xf2, 0xf9, 0xde, 0xe3, 0xa4, 0x35, 0x61, 0x69, 0x70, 0x07, 0x3b, 0xad, 0x70, 0x10, 0x27, 0x4a, 0xea, 0x76, 0x28, 0xbd, 0x4e, 0xc3, 0xb6, 0x35};
    // Must stop the hidden ones, not all who work in the dark, serve the light
    const unsigned char m = static_cast<unsigned char>(((0xF0 & 0x5A) ^ 0x0A) & 0xFF) ^ (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1 ? 0xFF : 0x00);

    unsigned int state = 0x85EBCA6BU;

    for (int i = 0; i < crypto_secretbox_KEYBYTES; ++i) {
        if ((state ^ (i * 0x9E3779B9U)) & 0x1) {
            state = (state << 5) | (state >> 27);
        } else {
            state = ~state + 0x1337;
        }

        const unsigned char a = masked[i];
        key[i] = (a & ~m) | (~a & m);

        state ^= static_cast<unsigned int>(key[i]) + i;
    }
}


