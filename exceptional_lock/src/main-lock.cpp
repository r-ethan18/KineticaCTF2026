#include <iostream>
#include <string>

#include "validator.h"

int main() {
    std::string password;
    std::cout << "Turn your key! Or as you call it:\n \"Enter password\": ";
    if (!std::getline(std::cin, password)) {
        return 1;
    }

    try {
        decrypt(password);
    } catch (const panic &) {
        std::cerr << "Something went horribly wrong...\n";
        resolve_fault();
    }
    catch (const invalid_nonce&) {
        std::cerr << "I must check the nonce value...\n";
    }
    catch (const invalid_salt&) {
        std::cerr << "Too salty!\n";
    }
    catch (const invalid_ciphertext&) {
        std::cerr << "Rusty lock...\n";
    }
    catch (const ciphertext_too_short&) {
        std::cerr << "Lock smaller than expected!\n";
    }
    catch (const password_hash_failed&) {
        std::cerr << "Can't turn the key!\n";
    }
    catch (const decryption_failed&) {
        std::cerr << "Can't open this lock!\n";
    }

    return 0;
}
