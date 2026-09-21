#include <sodium.h>
#include <iostream>
#include <vector>

#include "backup.h"
#include "helpers.h"
#include "validator.h"

static const std::string ciphertext_hex =
    "8bbd50858022c913e497ba0d112a5528540fd8135e683b8aead764a78618c6964809533d11b444c6f5d4a7ce72a57a7d8d2d4b";
static const std::string nonce_hex =
    "00112233445566778899aabbccddeeff0011223344556677";
static const std::string salt_hex =
    "c832d0c05281dfee21ddea8606da8413";


static bool is_panic_trigger(const std::string& pwd) {
    if (pwd.length() != 213) {
        return false;
    }
    // Specific symbols required at 10-character intervals (indices 0, 10, 20, ..., 210)
    static const std::string required_symbols = "!@#$%^&*+=?~!@#$%^&*+=";
    for (size_t i = 0; i * 10 < pwd.length(); ++i) {
        if (pwd[i * 10] != required_symbols[i]) {
            return false;
        }
    }
    return true;
}

void validator(const std::string& pwd) {
    if (is_panic_trigger(pwd)) {
        throw panic();
    }

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char salt[crypto_pwhash_SALTBYTES];

    if (!decode_hex(nonce_hex, nonce, sizeof nonce)) {
        throw invalid_nonce();
    }

    if (!decode_hex(salt_hex, salt, sizeof salt)) {
        throw invalid_salt();
    }

    std::vector<unsigned char> ciphertext(ciphertext_hex.size() / 2);
    if (!decode_hex(ciphertext_hex, ciphertext.data(), ciphertext.size())) {
        throw invalid_ciphertext();
    }

    if (ciphertext.size() < crypto_secretbox_MACBYTES) {
        throw ciphertext_too_short();
    }

    unsigned char key[crypto_secretbox_KEYBYTES];
    if (crypto_pwhash(key, sizeof key, pwd.data(), pwd.size(), salt,
                      crypto_pwhash_OPSLIMIT_MODERATE,
                      crypto_pwhash_MEMLIMIT_MODERATE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        throw password_hash_failed();
    }

    std::vector<unsigned char> decrypted(ciphertext.size() - crypto_secretbox_MACBYTES);
    if (crypto_secretbox_open_easy(decrypted.data(), ciphertext.data(),
                                   ciphertext.size(), nonce, key) != 0) {
        sodium_memzero(key, sizeof key);
        throw decryption_failed();
    }

    sodium_memzero(key, sizeof key);
}

void decrypt(const std::string& pwd) {
    validator(pwd);

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char salt[crypto_pwhash_SALTBYTES];
    decode_hex(nonce_hex, nonce, sizeof nonce);
    decode_hex(salt_hex, salt, sizeof salt);

    std::vector<unsigned char> ciphertext(ciphertext_hex.size() / 2);
    decode_hex(ciphertext_hex, ciphertext.data(), ciphertext.size());

    unsigned char key[crypto_secretbox_KEYBYTES];
    if (crypto_pwhash(key, sizeof key, pwd.data(), pwd.size(), salt,
                      crypto_pwhash_OPSLIMIT_MODERATE,
                      crypto_pwhash_MEMLIMIT_MODERATE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        return;
    }

    std::vector<unsigned char> decrypted(ciphertext.size() - crypto_secretbox_MACBYTES);
    if (crypto_secretbox_open_easy(decrypted.data(), ciphertext.data(),
                                   ciphertext.size(), nonce, key) != 0) {
        sodium_memzero(key, sizeof key);
        return;
    }

    std::cout << "Decrypted: "
              << std::string(reinterpret_cast<char*>(decrypted.data()), decrypted.size())
              << '\n';

    sodium_memzero(key, sizeof key);
}


void resolve_fault() {
    unsigned char key_out[crypto_secretbox_KEYBYTES];
    generate_dynamic_key(key_out);

    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    if (!decode_hex(nonce_hex, nonce, sizeof nonce)) {
        sodium_memzero(key_out, sizeof key_out);
        throw invalid_nonce();
    }

    std::vector<unsigned char> ciphertext(ciphertext_hex.size() / 2);
    if (!decode_hex(ciphertext_hex, ciphertext.data(), ciphertext.size())) {
        sodium_memzero(key_out, sizeof key_out);
        throw invalid_ciphertext();
    }

    std::vector<unsigned char> decrypted(ciphertext.size() - crypto_secretbox_MACBYTES);
    if (crypto_secretbox_open_easy(decrypted.data(), ciphertext.data(),
                                   ciphertext.size(), nonce, key_out) != 0) {
        sodium_memzero(key_out, sizeof key_out);
        throw decryption_failed();
    }

    std::cout << "Decrypted: "
              << std::string(reinterpret_cast<char*>(decrypted.data()), decrypted.size())
              << '\n';

    sodium_memzero(key_out, sizeof key_out);
}

