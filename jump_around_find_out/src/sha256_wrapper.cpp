#include "sha256_wrapper.h"
#include "picosha2.h"
#include <cstring>
#include <string>

extern "C" void picosha2_hash256_hex_string(const char *input, size_t len, char output_hex[65]) {
    std::string str_input(input, len);
    std::string hex_str;
    picosha2::hash256_hex_string(str_input, hex_str);
    std::strncpy(output_hex, hex_str.c_str(), 64);
    output_hex[64] = '\0';
}
