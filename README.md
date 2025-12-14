[![tests](https://github.com/aalp75/big-integer/actions/workflows/tests.yml/badge.svg?branch=master)](https://github.com/aalp75/big-integer/actions/workflows/tests.yml)

## BigInteger — Arbitrary-Precision Integer Class in C++

`BigInteger` is a C++ class implementing arbitrarily large signed integers (limited only by available memory).  

It supports:

- Unlimited integer size
- Full operator overloading (`+`, `-`, `*`, `/`, `%`, comparisons, stream I/O, etc.)
- Base-2³² internal representation (`uint32_t` digits)
- Knuth’s long division algorithm (TAOCP 4.3.1)

---

## Operation Complexity

Let **w** be the number of 32-bit words in the internal representation.

| Operation      | Complexity | Notes |
|----------------|------------|-------|
| Addition       | **O(w)**   | Classical schoolbook addition |
| Subtraction    | **O(w)**   | Classical schoolbook substraction |
| Multiplication | **O(w²)**  | Classical schoolbook multiplication |
| Division       | **O(w²)**  | Knuth Algorithm D (TAOCP Vol. 2 §4.3.1) |

---

## Code Example

```cpp
#include <iostream>
#include "BigInteger.h"

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
