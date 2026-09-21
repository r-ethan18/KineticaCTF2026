#include <sodium.h>
#include <iostream>
#include <vector>
#inc

int main() {

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

    return 0;
}
