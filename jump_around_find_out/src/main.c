#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sha256_wrapper.h"

// Must check carefully!
const char EXPECTED_HASH_0[] = "ecebe9dfe1aa99e55029c50d8d8c8038530f157dbbf2c0cd42f9a2969ec6732b";
const char EXPECTED_HASH_1[] = "c23808cb02b93de5a8befe1678937e30d4633bec2fd076f7667cb5c47ae063bb";
const char EXPECTED_HASH_2[] = "0ac0bd52c1dc5c1275adbf75b58d74d36170e208d80fbe257172a75d5bd57c51";
const char EXPECTED_HASH_3[] = "21cc6294913987c2a9c7648257142abcc3e00443344c1c2d3732ab0c46ec9d29";
const char EXPECTED_HASH_4[] = "6c9193f62c19d6d248a0c8550e74f287ed2092364959377a9e513451c18959b4";
const char EXPECTED_HASH_5[] = "1d81b8dacbe1a7e56705effaa8c2ce5f42bc88de26cfed4540618709358872fe";

char cumulative_state[256] = "";
size_t jump_history[32];
size_t jump_history_count = 0;

// Check again!!!
bool step_0_matched = false;
bool step_1_matched = false;
bool step_2_matched = false;
bool step_3_matched = false;
bool step_4_matched = false;
bool step_5_matched = false;

void update_state_matching(void) {
    step_0_matched = false;
    step_1_matched = false;
    step_2_matched = false;
    step_3_matched = false;
    step_4_matched = false;
    step_5_matched = false;
    char hash_out[65];
    size_t state_len = strlen(cumulative_state);

    if (state_len >= 6) {
        picosha2_hash256_hex_string(cumulative_state, 6, hash_out);
        if (strcmp(hash_out, EXPECTED_HASH_0) == 0) step_0_matched = true;
    }
    if (state_len >= 12 && step_0_matched) {
        picosha2_hash256_hex_string(cumulative_state, 12, hash_out);
        if (strcmp(hash_out, EXPECTED_HASH_1) == 0) step_1_matched = true;
    }
    if (state_len >= 18 && step_1_matched) {
        picosha2_hash256_hex_string(cumulative_state, 18, hash_out);
        if (strcmp(hash_out, EXPECTED_HASH_2) == 0) step_2_matched = true;
    }
    if (state_len >= 24 && step_2_matched) {
        picosha2_hash256_hex_string(cumulative_state, 24, hash_out);
        if (strcmp(hash_out, EXPECTED_HASH_3) == 0) step_3_matched = true;
    }
    if (state_len >= 30 && step_3_matched) {
        picosha2_hash256_hex_string(cumulative_state, 30, hash_out);
        if (strcmp(hash_out, EXPECTED_HASH_4) == 0) step_4_matched = true;
    }
    if (state_len >= 36 && step_4_matched) {
        picosha2_hash256_hex_string(cumulative_state, 36, hash_out);
        if (strcmp(hash_out, EXPECTED_HASH_5) == 0) step_5_matched = true;
    }
}

void jump_to_venice(void) {
    char scrambled[6] = { 0x61, 0x53, 0x74, 0x6c, 0x72, 0x61 };
    char restored[6];
    restored[0] = scrambled[1];
    restored[1] = scrambled[0];
    restored[2] = scrambled[3];
    restored[3] = scrambled[2];
    restored[4] = scrambled[5];
    restored[5] = scrambled[4];
    size_t len = strlen(cumulative_state);
    if (len + 6 < sizeof(cumulative_state)) {
        memcpy(cumulative_state + len, restored, 6);
        cumulative_state[len + 6] = '\0';
        if (jump_history_count < 32) {
            jump_history[jump_history_count++] = 6;
        }
        update_state_matching();
    }
}

void jump_to_naples(void) {
    char scrambled[6] = { 0x69, 0x69, 0x63, 0x66, 0x66, 0x6c };
    char restored[6];
    restored[0] = scrambled[4];
    restored[1] = scrambled[3];
    restored[2] = scrambled[0];
    restored[3] = scrambled[2];
    restored[4] = scrambled[1];
    restored[5] = scrambled[5];
    size_t len = strlen(cumulative_state);
    if (len + 6 < sizeof(cumulative_state)) {
        memcpy(cumulative_state + len, restored, 6);
        cumulative_state[len + 6] = '\0';
        if (jump_history_count < 32) {
            jump_history[jump_history_count++] = 6;
        }
        update_state_matching();
    }
}


void jump_to_palermo(void) {
    char scrambled[6] = { 0x5f, 0x73, 0x6e, 0x6f, 0x6f, 0x6f };
    char restored[6];
    restored[0] = scrambled[5];
    restored[1] = scrambled[0];
    restored[2] = scrambled[1];
    restored[3] = scrambled[4];
    restored[4] = scrambled[2];
    restored[5] = scrambled[3];
    size_t len = strlen(cumulative_state);
    if (len + 6 < sizeof(cumulative_state)) {
        memcpy(cumulative_state + len, restored, 6);
        cumulative_state[len + 6] = '\0';
        if (jump_history_count < 32) {
            jump_history[jump_history_count++] = 6;
        }
        update_state_matching();
    }
}

void jump_to_milan(void) {
    char scrambled[6] = { 0x6f, 0x5f, 0x69, 0x72, 0x4d, 0x61 };
    char restored[6];
    restored[0] = scrambled[1];
    restored[1] = scrambled[4];
    restored[2] = scrambled[5];
    restored[3] = scrambled[3];
    restored[4] = scrambled[2];
    restored[5] = scrambled[0];
    size_t len = strlen(cumulative_state);
    if (len + 6 < sizeof(cumulative_state)) {
        memcpy(cumulative_state + len, restored, 6);
        cumulative_state[len + 6] = '\0';
        if (jump_history_count < 32) {
            jump_history[jump_history_count++] = 6;
        }
        update_state_matching();
    }
}

void jump_to_florence(void) {
    char scrambled[6] = { 0x65, 0x5f, 0x64, 0x65, 0x5f, 0x69 };
    char restored[6];
    restored[0] = scrambled[0];
    restored[1] = scrambled[1];
    restored[2] = scrambled[3];
    restored[3] = scrambled[4];
    restored[4] = scrambled[2];
    restored[5] = scrambled[5];
    size_t len = strlen(cumulative_state);
    if (len + 6 < sizeof(cumulative_state)) {
        memcpy(cumulative_state + len, restored, 6);
        cumulative_state[len + 6] = '\0';
        if (jump_history_count < 32) {
            jump_history[jump_history_count++] = 6;
        }
        update_state_matching();
    }
}

void jump_to_verona(void) {
    char scrambled[6] = { 0x5f, 0x61, 0x69, 0x6d, 0x5f, 0x65 };
    char restored[6];
    restored[0] = scrambled[5];
    restored[1] = scrambled[0];
    restored[2] = scrambled[3];
    restored[3] = scrambled[1];
    restored[4] = scrambled[4];
    restored[5] = scrambled[2];
    size_t len = strlen(cumulative_state);
    if (len + 6 < sizeof(cumulative_state)) {
        memcpy(cumulative_state + len, restored, 6);
        cumulative_state[len + 6] = '\0';
        if (jump_history_count < 32) {
            jump_history[jump_history_count++] = 6;
        }
        update_state_matching();
    }
}

// Made a mistake, yikes!
void undo_jump(void) {
    if (jump_history_count > 0) {
        size_t last_len = jump_history[--jump_history_count];
        size_t current_len = strlen(cumulative_state);
        if (current_len >= last_len) {
            cumulative_state[current_len - last_len] = '\0';
        }
        update_state_matching();
    }
}

int main(void) {
    printf("I must have forgotten to finish this...\n");
    printf("Press Enter to exit...\n");
    getchar();
    return 0;
}
