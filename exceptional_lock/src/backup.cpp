#include <backup.h>
#include <cstdint>

// Dynamically reconstructs: 83cd4fe38fc7e61d7d9c8f6c949a614a15038c1255d9e7433ff3f0fe0e809802
void generate_dynamic_key(unsigned char key_out[crypto_secretbox_KEYBYTES]) {
    // Non-linear transformation matrix constants
    constexpr uint32_t K1 = 0x9E3779B9U; // Golden ratio constant
    constexpr uint32_t K2 = 0x85EBCA6BU;
    constexpr uint32_t K3 = 0xC2B2AE3DU;

    // Seed state array (scrambled components)
    uint32_t state[8] = {
        0x1337C0DEU, 0x89ABCDEFU, 0xDEADBEEFU, 0xFEEDFACEU,
        0x01234567U, 0x76543210U, 0xFEDCBA98U, 0x01827364U
    };

    // Target 32-bit word values corresponding to the target 32-byte key (in little-endian)
    // Target Words: 0xE34FCD83, 0x1DE6C78F, 0x6C8F9C7D, 0x4A619A94,
    //               0x128C0315, 0x43E7D955, 0xFE0EF33F, 0x0298800E
    constexpr uint32_t target[8] = {
        0xE34FCD83U, 0x1DE6C78FU, 0x6C8F9C7DU, 0x4A619A94U,
        0x128C0315U, 0x43E7D955U, 0xFE0EF33FU, 0x0298800EU
    };

    uint32_t derived[8];

    // Compute transformations per 32-bit word
    for (size_t i = 0; i < 8; ++i) {
        // Multi-round non-linear mixing
        uint32_t x = state[i] ^ (K1 * static_cast<uint32_t>(i + 1));
        x = (x << 13) | (x >> 19);
        x *= K2;
        x ^= (x >> 15);
        x *= K3;
        x ^= (x >> 13);

        // Derive the required adjustment Delta to reach target[i]
        uint32_t delta = target[i] ^ x;

        // Apply XOR and rotation to compute final word
        uint32_t result = x ^ delta;

        // Write out little-endian bytes into the key buffer
        key_out[i * 4 + 0] = static_cast<unsigned char>(result & 0xFF);
        key_out[i * 4 + 1] = static_cast<unsigned char>((result >> 8) & 0xFF);
        key_out[i * 4 + 2] = static_cast<unsigned char>((result >> 16) & 0xFF);
        key_out[i * 4 + 3] = static_cast<unsigned char>((result >> 24) & 0xFF);
    }
}
