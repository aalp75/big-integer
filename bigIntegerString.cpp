#include <iostream>
#include <string>

class BigIntegerString {
private:
	int m_sign;
	std::string m_value;

public:
	BigIntegerString() : m_sign(0), m_value("0") {}

	BigIntegerString(int value) {
		
		if (value == 0) {
			m_sign = 0;
			m_value = "0";
			return;
		}

		m_value = std::to_string(value);

		if (value > 0) {
			m_sign = 1;
		}
		else {
			m_sign = 0;
		}
	}

	BigIntegerString(std::string value) : m_value(value) {
		if (m_value[0] == '-') {
			m_sign = -1;
		}
		else {
			m_sign = 1;
		}
	}

	void print() {
		if (m_sign == -1) {
			std::cout << "-";
		}
		std::cout << m_value << '\n';
	}
};


int main() {
	
	BigIntegerString b1;
	b1.print();
	BigIntegerString b2(42);
	b2.print();
	BigIntegerString b3("21432");
	b3.print();
	BigIntegerString b4("-1241243");
	b4.print();

	return 0;
}