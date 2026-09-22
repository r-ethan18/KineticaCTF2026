#include <string>
#include <iostream>

#include "picosha2.h"

// these chunks should actually be mangled
std::string chunk0 = "53616c746172";
std::string chunk1 = "3616c7461726";
std::string chunk2 = "616c74617265";
std::string chunk3 = "16c746172655";
std::string chunk4 = "6c746172655f";
std::string chunk5 = "c746172655f6";

std::string chunk0_hash;
std::string chunk1_hash;
std::string chunk2_hash;
std::string chunk3_hash;
std::string chunk4_hash;
std::string chunk5_hash;

int main() {
    // generate incremental hashes
    std::string accumulated_hex = "";
    picosha2::hash256_hex_string(accumulated_hex += chunk0, chunk0_hash);
    std::cout << accumulated_hex << std::endl;
    picosha2::hash256_hex_string(accumulated_hex += chunk1, chunk1_hash);
    std::cout << accumulated_hex << std::endl;
    picosha2::hash256_hex_string(accumulated_hex += chunk2, chunk2_hash);
    std::cout << accumulated_hex << std::endl;
    picosha2::hash256_hex_string(accumulated_hex += chunk3, chunk3_hash);
    std::cout << accumulated_hex << std::endl;
    picosha2::hash256_hex_string(accumulated_hex += chunk4, chunk4_hash);
    std::cout << accumulated_hex << std::endl;
    picosha2::hash256_hex_string(accumulated_hex += chunk5, chunk5_hash);
    std::cout << accumulated_hex << std::endl;

    // chunk<i>_hash => hash of concatentated hex strings until ith chunk
    std::cout << chunk0_hash << std::endl;
    std::cout << chunk1_hash << std::endl;
    std::cout << chunk2_hash << std::endl;
    std::cout << chunk3_hash << std::endl;
    std::cout << chunk4_hash << std::endl;
    std::cout << chunk5_hash << std::endl;

    return 0;
}
