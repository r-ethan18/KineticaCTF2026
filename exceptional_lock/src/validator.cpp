#include <sodium.h>
#include <iostream>
#include <vector>

#include "helpers.h"
#include "validator.h"

void decrypt(std::string pwd) {
    int message_length = 35;
    std::vector<unsigned char> decrypted(message_length);

    const std::string ciphertext_hex =
        "c317e60a64d593766205b8ac2ff06804b78c249e4e84c3551f92ede39594356064ed5c3181a844bf85d5bbd460019b8523b5be";
    const std::string nonce_hex =
        "00112233445566778899aabbccddeeff0011223344556677";
    const std::string salt_hex = "ec34c5f93acc5de95bb70fec4abed95";

    unsigned char key[crypto_secretbox_KEYBYTES];
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char salt[crypto_pwhash_SALTBYTES];

    if (!decode_hex(nonce_hex, nonce, sizeof nonce)) {
        std::cerr << "Invalid nonce. It must contain exactly "
                  << crypto_secretbox_NONCEBYTES * 2
                  << " hexadecimal characters.\n";
        exit(1);
    }

    if (!decode_hex(salt_hex, salt, sizeof salt)) {
        std::cerr << "Invalid nonce. It must contain exactly "
                  << crypto_secretbox_NONCEBYTES * 2
                  << " hexadecimal characters.\n";
        exit(1);
    }

    std::vector<unsigned char> ciphertext(
        ciphertext_hex.size() / 2
    );

    if (!decode_hex(
            ciphertext_hex,
            ciphertext.data(),
            ciphertext.size())) {
        std::cerr << "Invalid ciphertext\n";
        exit(1);
    }

    if (ciphertext.size() < crypto_secretbox_MACBYTES) {
        std::cerr << "Ciphertext is too short\n";
        exit(1);
    }

    std::vector<unsigned char> plaintext(ciphertext.size() -
                                         crypto_secretbox_MACBYTES);

    if (crypto_pwhash(key, sizeof key, pwd.data(), pwd.size(), salt,
                      crypto_pwhash_OPSLIMIT_MODERATE,
                      crypto_pwhash_MEMLIMIT_MODERATE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        std::cerr << "Password hashing failed\n";
        exit(1);
    }


    if (crypto_secretbox_open_easy(
            decrypted.data(),
            ciphertext.data(),
            ciphertext.size(),
            nonce,
            key) != 0) {
        std::cerr << "Decryption failed: wrong password, invalid nonce, "
                     "or modified ciphertext.\n";
        exit(1);
    }

    std::cout << "Decrypted:  "
              << std::string(
                     reinterpret_cast<char*>(decrypted.data()),
                     decrypted.size())
              << '\n';

    sodium_memzero(key, sizeof key);
}
