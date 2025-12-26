#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "bigInteger.h"

using BI = BigInteger;

int main() {

	BigInteger u("144659075040145311177374690267887710956");
	BigInteger v("56343573548904472842532843271");

	u.printWords();
	v.printWords();

	BigInteger d = u / v;
	std::cout << "q = " << d << '\n';
	std::cout << "r = " << u % v << '\n';
	return 0;

	std::vector<uint32_t> v1;
	std::vector<uint32_t> v2;

	std::mt19937_64 rng(42);

	uint64_t BASE = (1LL << 32);

	BigInteger x, y;

	// random big numbers
	for (int i = 0; i < 10; i++) {
		v1.clear();
		v2.clear();
		for (int j = 0; j < 200; j++) {
			v1.push_back(static_cast<uint32_t>(rng() % BASE));
			v2.push_back(static_cast<uint32_t>(rng() % BASE));	
		}
		x = BigInteger(v1);
		y = BigInteger(v2);

		BigInteger expected = x * y;
		BigInteger res = karatsubaMultiplication(x, y);
	}

	using clock = std::chrono::steady_clock;
	auto t0 = clock::now();

	for (int i = 0; i < 1000; i++) {
		BigInteger res = multiplicationAbsolute(x, y);
	}

	auto t1 = clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << "School book multplication - Elapsed: " << ms << " milliseconds\n";


	using clock = std::chrono::steady_clock;
	t0 = clock::now();

	for (int i = 0; i < 1000; i++) {
		BigInteger res = karatsubaMultiplication(x, y);
	}

	t1 = clock::now();
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << "Karatsuba multplication - Elapsed: " << ms << " milliseconds\n";
}