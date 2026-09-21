#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

// Lightweight procedural exception structures
struct panic {};
struct invalid_nonce {};
struct invalid_salt {};
struct invalid_ciphertext {};
struct ciphertext_too_short {};
struct password_hash_failed {};
struct decryption_failed {};

void validator(const std::string& pwd);
void decrypt(const std::string& pwd);
void resolve_fault();

#endif
