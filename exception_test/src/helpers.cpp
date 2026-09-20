#include "helpers.h"
#include <iostream>

void print_hex(const char* label,
               const unsigned char* data,
               std::size_t length)
{
    std::string hex(length * 2 + 1, '\0');

    sodium_bin2hex(
        hex.data(),
        hex.size(),
        data,
        length
    );

    std::cout << label << hex.c_str() << '\n';
}

bool decode_hex(const std::string& hex,
                unsigned char* output,
                std::size_t output_length)
{
    // Each byte requires two hexadecimal characters.
    if (hex.size() != output_length * 2) {
        return false;
    }

    return sodium_hex2bin(
        output,
        output_length,
        hex.c_str(),
        hex.size(),
        nullptr,
        nullptr,
        nullptr
    ) == 0;
}
