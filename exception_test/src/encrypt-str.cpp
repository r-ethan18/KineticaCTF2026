#include <sodium.h>

#include <iostream>
#include <string>
#include <vector>

#include "helpers.h"

int main()
{
    if (sodium_init() < 0) {
        return 1;
    }

    const std::string password =
        "correct horse battery staple";

    const std::string message =
        "test";

    /*
     * A custom 24-byte nonce represented as 48 hexadecimal characters.
     *
     * Never reuse the same nonce with the same password-derived key.
     */
    const std::string nonce_hex =
        "00112233445566778899aabbccddeeff0011223344556677";

    unsigned char nonce[crypto_secretbox_NONCEBYTES];

    if (!decode_hex(nonce_hex, nonce, sizeof nonce)) {
        std::cerr << "Invalid nonce. It must contain exactly "
                  << crypto_secretbox_NONCEBYTES * 2
                  << " hexadecimal characters.\n";
        return 1;
    }

    /*
     * The salt is not secret. It must be stored with the ciphertext
     * and reused when deriving the key during decryption.
     */
    unsigned char salt[crypto_pwhash_SALTBYTES];
    randombytes_buf(salt, sizeof salt);

    unsigned char key[crypto_secretbox_KEYBYTES];

    if (crypto_pwhash(
            key,
            sizeof key,
            password.data(),
            password.size(),
            salt,
            crypto_pwhash_OPSLIMIT_MODERATE,
            crypto_pwhash_MEMLIMIT_MODERATE,
            crypto_pwhash_ALG_DEFAULT) != 0) {
        std::cerr << "Password hashing failed\n";
        return 1;
    }

    const auto* plaintext =
        reinterpret_cast<const unsigned char*>(message.data());

    const std::size_t message_length = message.size();

    /*
     * crypto_secretbox_easy() puts the MAC at the beginning of
     * the ciphertext buffer.
     */
    std::vector<unsigned char> ciphertext(
        crypto_secretbox_MACBYTES + message_length
    );

    if (crypto_secretbox_easy(
            ciphertext.data(),
            plaintext,
            message_length,
            nonce,
            key) != 0) {
        std::cerr << "Encryption failed\n";
        return 1;
    }

    print_hex("Salt:       ", salt, sizeof salt);
    print_hex("Nonce:      ", nonce, sizeof nonce);
    print_hex("Ciphertext: ", ciphertext.data(), ciphertext.size());

    /*
     * Decryption.
     *
     * In a real application, these values would be loaded from storage
     * rather than reused directly from memory.
     */
    std::vector<unsigned char> decrypted(message_length);

    if (crypto_secretbox_open_easy(
            decrypted.data(),
            ciphertext.data(),
            ciphertext.size(),
            nonce,
            key) != 0) {
        std::cerr << "Decryption failed: wrong password, invalid nonce, "
                     "or modified ciphertext.\n";
        return 1;
    }

    std::cout << "Decrypted:  "
              << std::string(
                     reinterpret_cast<char*>(decrypted.data()),
                     decrypted.size())
              << '\n';

    sodium_memzero(key, sizeof key);

    return 0;
}
