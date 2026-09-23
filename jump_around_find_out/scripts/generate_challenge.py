#!/usr/bin/env python3
import hashlib
import random

# Target secret payload
TARGET_STRING = "Saltare_e_difficile_ma_io_sono_Mario"
CHUNK_SIZE = 6

CITIES = [
    ("venice", 0),
    ("florence", 1),
    ("naples", 2),
    ("verona", 3),
    ("palermo", 4),
    ("milan", 5)
]

SEED_BASE = 1337

def sha256_hex(data_str):
    return hashlib.sha256(data_str.encode('utf-8')).hexdigest()

def generate():
    chunks = [TARGET_STRING[i:i+CHUNK_SIZE] for i in range(0, len(TARGET_STRING), CHUNK_SIZE)]
    assert len(chunks) == 6, f"Expected 6 chunks, got {len(chunks)}"

    # Pre-compute rolling hashes for each prefix step
    rolling_hashes = []
    accum = ""
    for i, chunk in enumerate(chunks):
        accum += chunk
        h = sha256_hex(accum)
        rolling_hashes.append(h)
        print(f"Step {i} ({accum}): {h}")

    # Generate C code
    c_code = []
    c_code.append('/* Auto-generated C CTF Challenge source */')
    c_code.append('#include <stdio.h>')
    c_code.append('#include <string.h>')
    c_code.append('#include <stdbool.h>')
    c_code.append('#include "sha256_wrapper.h"')
    c_code.append('')

    # Declare expected hash constants
    c_code.append('/* Expected rolling SHA-256 hashes for each step */')
    for i, h in enumerate(rolling_hashes):
        c_code.append(f'const char EXPECTED_HASH_{i}[] = "{h}";')
    c_code.append('')

    # Global tracking variables
    c_code.append('/* Global cumulative state and history stack */')
    c_code.append('char cumulative_state[256] = "";')
    c_code.append('size_t jump_history[32];')
    c_code.append('size_t jump_history_count = 0;')
    c_code.append('')
    c_code.append('/* Step match flags */')
    for i in range(6):
        c_code.append(f'bool step_{i}_matched = false;')
    c_code.append('')

    # State update helper function
    c_code.append('void update_state_matching(void) {')
    for i in range(6):
        c_code.append(f'    step_{i}_matched = false;')
    c_code.append('    char hash_out[65];')
    c_code.append('    size_t state_len = strlen(cumulative_state);')
    c_code.append('')
    
    for i in range(6):
        req_len = (i + 1) * CHUNK_SIZE
        prev_cond = f" && step_{i-1}_matched" if i > 0 else ""
        c_code.append(f'    if (state_len >= {req_len}{prev_cond}) {{')
        c_code.append(f'        picosha2_hash256_hex_string(cumulative_state, {req_len}, hash_out);')
        c_code.append(f'        if (strcmp(hash_out, EXPECTED_HASH_{i}) == 0) step_{i}_matched = true;')
        c_code.append('    }')
    
    c_code.append('}')
    c_code.append('')

    # Generate jump functions in C
    for city, idx in CITIES:
        chunk = chunks[idx]
        chunk_bytes = [ord(c) for c in chunk]

        # PRNG shuffle
        rng = random.Random(SEED_BASE + idx)
        indices = list(range(CHUNK_SIZE))
        rng.shuffle(indices)

        scrambled_bytes = [0] * CHUNK_SIZE
        for orig_pos, scram_pos in enumerate(indices):
            scrambled_bytes[scram_pos] = chunk_bytes[orig_pos]

        c_code.append(f'/* Jump function for Chunk {idx}: "{chunk}" */')
        c_code.append(f'void jump_to_{city}(void) {{')
        c_code.append(f'    /* Mangled storage with fixed PRNG seed ({SEED_BASE + idx}) */')
        scrambled_hex = ", ".join([f"0x{b:02x}" for b in scrambled_bytes])
        c_code.append(f'    char scrambled[{CHUNK_SIZE}] = {{ {scrambled_hex} }};')
        c_code.append(f'    char restored[{CHUNK_SIZE}];')
        
        for orig_pos, scram_pos in enumerate(indices):
            c_code.append(f'    restored[{orig_pos}] = scrambled[{scram_pos}];')

        c_code.append(f'    size_t len = strlen(cumulative_state);')
        c_code.append(f'    if (len + {CHUNK_SIZE} < sizeof(cumulative_state)) {{')
        c_code.append(f'        memcpy(cumulative_state + len, restored, {CHUNK_SIZE});')
        c_code.append(f'        cumulative_state[len + {CHUNK_SIZE}] = \'\\0\';')
        c_code.append(f'        if (jump_history_count < 32) {{')
        c_code.append(f'            jump_history[jump_history_count++] = {CHUNK_SIZE};')
        c_code.append(f'        }}')
        c_code.append(f'        update_state_matching();')
        c_code.append(f'    }}')
        c_code.append(f'}}')
        c_code.append('')

    # Undo/Revert function in C
    c_code.append('/* Revert function to roll back the last traversal step */')
    c_code.append('void undo_jump(void) {')
    c_code.append('    if (jump_history_count > 0) {')
    c_code.append('        size_t last_len = jump_history[--jump_history_count];')
    c_code.append('        size_t current_len = strlen(cumulative_state);')
    c_code.append('        if (current_len >= last_len) {')
    c_code.append('            cumulative_state[current_len - last_len] = \'\\0\';')
    c_code.append('        }')
    c_code.append('        update_state_matching();')
    c_code.append('    }')
    c_code.append('}')
    c_code.append('')

    # Main entry point in C
    c_code.append('int main(void) {')
    c_code.append('    printf("=== Jump Around & Find Out CTF Challenge ===\\n");')
    c_code.append('    printf("Target sequence of 6 jump functions must be invoked in order.\\n");')
    c_code.append('    printf("Use a debugger (e.g. GDB) to call functions and inspect step match flags.\\n");')
    c_code.append('    printf("Press Enter to exit...\\n");')
    c_code.append('    getchar();')
    c_code.append('    return 0;')
    c_code.append('}')
    c_code.append('')

    output_path = "src/main.c"
    with open(output_path, "w") as f:
        f.write("\n".join(c_code))
    print(f"Successfully generated {output_path}")

if __name__ == "__main__":
    generate()
