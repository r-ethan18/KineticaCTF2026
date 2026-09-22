#include <string>
#include <iostream>

int main() {
    std::string chunk0 = "";
    chunk0 += (char)0x53;
    chunk0 += (char)0x61;
    chunk0 += (char)0x6c;
    chunk0 += (char)0x74;
    chunk0 += (char)0x61;
    chunk0 += (char)0x72;

    std::cout << chunk0 << std::endl;
}
