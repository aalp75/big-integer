#include <iostream>
#include <vector>
#include <string>

class BigInteger {
private:
	
	int sign; // -1, 0, or 1
	std::vector<uint32_t> mag; // form in base 2 ^ 32

public:

	BigInteger() : sign(1) {}

	BigInteger(int value) {
		if (value == 0) {
			sign = 0;
			return;
		}
		if (value > 0) {
			sign = 1;
			mag.push_back(value);
		}
		else {
			sign = -1;
			mag.push_back(value);
		}
	}

	BigInteger(std::string s) {
		// for example 2^42
		// 2^32 + 2 ^ 10
		std::cout << "here" << std::endl;
	}	

	void print() {
		std::cout << "sign: " << sign << '\n';
		std::cout << "array length: " << mag.size() << '\n';
		for (auto e : mag) {
			std::cout << e << " ";
		}
		std::cout << '\n';
	}
};

int main() {

	BigInteger b(42);
	b.print();

	BigInteger("123");

	return 0;
}