#include <iostream>
#include <vector>
#include <chrono>
#include "bigInteger.h"

using BI = BigInteger;

int main() {

	using clock = std::chrono::steady_clock;
	

	std::string s1;
	for (int i = 0; i < 15; i++) {
		s1 += "41878979874271489721438972143897214089721412748217474331415926535897932384626433832795";
	}

	std::string s2;
	for (int i = 0; i < 15; i++) {
		s2 += "2718281828459045235369889748274812748943987214897214897218947127412743144432";
	}

	std::cout << "len(s1) = " << s1.size() << "\nlen(s2) = " << s2.size() << '\n';

	BI b1(s1);
	BI b2(s2);

	std::cout << "b1 = " << b1 << "\n\nb2 = " << b2 << '\n';

	auto t0 = clock::now();

	BI res = b1 * b2;
	//auto res2 = b1 % b2;

	std::cout << "\n\nres = " << res << '\n';
	//std::cout << "r = " << res2 << '\n';

	auto t1 = clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    std::cout << "Elapsed: " << ms << " ms\n";
}