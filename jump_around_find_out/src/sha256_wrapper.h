#ifndef SHA256_WRAPPER_H
#define SHA256_WRAPPER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void picosha2_hash256_hex_string(const char *input, size_t len, char output_hex[65]);

#ifdef __cplusplus
}
#endif

#endif // SHA256_WRAPPER_H
