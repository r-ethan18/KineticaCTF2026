#ifndef HELPERS_H
#define HELPERS_H

#include <sodium.h>
#include <string>

void print_hex(const char *label,
               const unsigned char *data,
               std::size_t length);

bool decode_hex(const std::string& hex,
                unsigned char* output,
                std::size_t output_length);

#endif
