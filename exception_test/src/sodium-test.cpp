#include <sodium.h>
#include <iostream>

int main(void)
{
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized; it is not safe to use */
    }
    std::cout << "Bruh" << std::endl;
    return 0;
}
