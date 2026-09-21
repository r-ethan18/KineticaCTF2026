#include <iostream>
#include <string>

#include "validator.h"

// Lightweight trigger type
struct panic {};

void run_secret_path(const std::string& input) {
    std::cout << "[+] Secret path triggered via exception!\n";
    // Call generate_dynamic_key() / decryption here
}

void validate_input(const std::string& input) {
    if (input.length() > 167) {
        throw panic(); // Throwing your 'panic' exception
    }
}

int main() {
    std::string password;
    std::cout << "Enter password: ";
    std::cin >> password;

    try {
        decrypt(password);
        std::cout << "Normal validation path.\n";
    }
    catch (const panic&) { // Catching 'panic' specifically
        std::cout << "[!] Caught panic exception!\n";
        run_secret_path(password);
    }

    return 0;
}
