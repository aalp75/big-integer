#include <iostream>
#include "bigInteger.h"


int main() {

	/*int x = 4213;
	BI b1(x);
	std::cout << b1 << '\n';

	long long v = 1124124312412431243;
	std::cout << "Initial: " << v << '\n';
	BI b2(v);

	std::cout << b2 << '\n';
	std::cout << 1ll * b2.m_digits[0] * (1l << 32) + b2.m_digits[1] << '\n';

	std::string input("2184821741287438127409874238921743182743128743");
	std::cout << "input: " << input << '\n';
	BI b5(input);
	std::cout << b5 << '\n';

	std::string s = b5.toString();
	if (s == input) {
		std::cout << "Check: GOOD\n";
	}
	else {
		std::cout << "Check: BAD\n";
	}

	BI b6("12412421412421421431241243124312431243");
	BI b7("124124124312431243343134212431243124312431243124312431243");

	b6.printClass();
	b7.printClass();

	std::cout << b6 << '\n';
	std::cout << b7 << '\n';
	*/

	//BI b8 = additionAbsolute(b6, b7);

	//std::cout << b8 << '\n';

	//BI b9 = substractionAbsolute(b7, b6);
	//std::cout << b9 << '\n';

	//std::cout << "** Performing multiplication **\n";
	//BI b10 = multiplicationAbsolute(b6, b7);
	//std::cout << b10 << '\n';


	//std::cout << "** RUN SHORT DIVISION **\n";
	//BI b14("1241294092184091284092181286747892174872143843");
	//BI b15("2374872");

	//debug(b15.m_digits);

	//auto [q, r] = shortDivision(b14, b15);
	//std:cout << "q = " << q << '\n';
	//std::cout << "r = " << r << '\n';

	std::cout << "** RUN DIVISION **\n";
	BI b11("340282366920938463463374607431768211456");
	BI b12("18446744073709551616");

	//BI b11("140143607691181373357373357406387550411018827911774091176114398363648");
	//BI b12("15624580915668123648");

	//debug(b12.m_digits);
	//return 0;
	auto [q2, r2] = divideAndRemainder(b11, b12);

	std::cout << "q = " << q2 << '\n';
	std::cout << "r = " << r2 << '\n';

	return 0;
}