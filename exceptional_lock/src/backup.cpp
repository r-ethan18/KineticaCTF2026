#include "backup.h"
#include <sys/ptrace.h>

// void generate_dynamic_key(unsigned char key[crypto_secretbox_KEYBYTES]) {
//     static const unsigned char masked[] = {0xd9, 0x97, 0x15, 0xb9, 0xd5, 0x9d, 0xbc, 0x47, 0x27, 0xc6, 0xd5, 0x36, 0xce, 0xc0, 0x3b, 0x10, 0x4f, 0x59, 0xd6, 0x48, 0xf, 0x83, 0xbd, 0x19, 0x65, 0xa9, 0xaa, 0xa4, 0x54, 0xda, 0xc2, 0x58};

//     const unsigned char mask = 0x5A;
//     for (int i = 0; i < crypto_secretbox_KEYBYTES; ++i) {
//         key[i] = masked[i] ^ mask;
//     }
// }

void generate_dynamic_key(unsigned char key[crypto_secretbox_KEYBYTES]) {
    // Recalculated for key: "f2c42b8e5407b61ba8a384b9fe6f3b332a5d61f72a4a7d10b02c72e71499ec6f"
    // Mask: 0x5A

    static const unsigned char masked[] = {0xa8, 0x9e, 0x71, 0xd4, 0x0e, 0x5d, 0xec, 0x41, 0xf2, 0xf9, 0xde, 0xe3, 0xa4, 0x35, 0x61, 0x69, 0x70, 0x07, 0x3b, 0xad, 0x70, 0x10, 0x27, 0x4a, 0xea, 0x76, 0x28, 0xbd, 0x4e, 0xc3, 0xb6, 0x35};

    // Evaluates to 0x5A if no debugger is attached; 0xA5 if debugger is attached via ptrace
    const unsigned char m = static_cast<unsigned char>(((0xF0 & 0x5A) ^ 0x0A) & 0xFF) ^ (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1 ? 0xFF : 0x00);

    unsigned int state = 0x85EBCA6BU;

    for (int i = 0; i < crypto_secretbox_KEYBYTES; ++i) {
        if ((state ^ (i * 0x9E3779B9U)) & 0x1) {
            state = (state << 5) | (state >> 27);
        } else {
            state = ~state + 0x1337;
        }

        // Bitwise XOR transformation using (a & ~m) | (~a & m)
        const unsigned char a = masked[i];
        key[i] = (a & ~m) | (~a & m);

        state ^= static_cast<unsigned int>(key[i]) + i;
    }
}


