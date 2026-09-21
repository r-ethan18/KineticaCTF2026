#include <iostream>
#include <string>

#include "backup.h"
#include "helpers.h"
#include "validator.h"

int main() {
    std::string password;
    std::cout << "Enter password: ";
    if (!std::getline(std::cin, password)) {
        return 1;
    }

    try {
        decrypt(password);
    }
    catch (const panic&) {
        resolve_fault();
    }
    catch (const invalid_nonce&) {
        std::cerr << "Invalid nonce.\n";
    }
    catch (const invalid_salt&) {
        std::cerr << "Invalid salt.\n";
    }
    catch (const invalid_ciphertext&) {
        std::cerr << "Invalid ciphertext.\n";
    }
    catch (const ciphertext_too_short&) {
        std::cerr << "Ciphertext too short.\n";
    }
    catch (const password_hash_failed&) {
        std::cerr << "Password hashing failed.\n";
    }
    catch (const decryption_failed&) {
        std::cerr << "Decryption failed: wrong password.\n";
    }

    return 0;
}
