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
    BigInteger b1("314159265358979323846264338327950288419716939937510");
    BigInteger b2("57721566490153286060651209008240");

    BigInteger b3 = b1 + b2; 
    BigInteger b4 = b1 - b2; 
    BigInteger b5 = b1 * b2; 
    BigInteger b6 = b1 / b2;
    BigInteger b7 = b1 % b2;

    std::cout << b3 << std::endl; // 314159265358979323903985904818103574480368148945750
    std::cout << b4 << std::endl; // 314159265358979323788542771837797002359065730929270
    std::cout << b5 << std::endl; // 18133764923916034996131345312710323817634758445161466273272003379428833534675082400
    std::cout << b6 << std::endl; // 5442667004066352026
    std::cout << b7 << std::endl; // 21269480725413109709710765243270

    return 0;
}
