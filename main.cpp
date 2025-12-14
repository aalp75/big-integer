#include<iostream>
#include<vector>
#include "bigInteger.h"

using BI = BigInteger;


int main() {

	BI b1("-31415926535897932384626433832795");
	BI b2("27182818284590452353602874713527");

	std::cout << "b1 = " << b1 << "\nb2 = " << b2 << '\n';

	b1.printDigits();
	
	auto res = b1 + b2;
	//auto res2 = b1 % b2;

	std::cout << "res = " << res << '\n';
	//std::cout << "r = " << res2 << '\n';
}