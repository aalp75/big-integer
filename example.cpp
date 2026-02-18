#include <iostream>
#include "bigInteger.h"

int main() {
    BigInteger b1("3141592653589793238462643");
    BigInteger b2("577215664901532860606");

    BigInteger b3 = b1 + b2;
    BigInteger b4 = b1 - b2;
    BigInteger b5 = b1 * b2;
    BigInteger b6 = b1 / b2;
    BigInteger b7 = b1 % b2;

    std::cout << b3 << std::endl; // 3142169869254694771323249
    std::cout << b4 << std::endl; // 3141015437924891705602037
    std::cout << b5 << std::endl; // 1813376492391603499611525531595255249457341658
    std::cout << b6 << std::endl; // 5442
    std::cout << b7 << std::endl; // 385005195651411044791

    return 0;
}