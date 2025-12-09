#include <iostream>
#include <vector>
#include <string>

const static long long SHIFT = 32;
const static long long BASE = 1ll << SHIFT;
const static long long BASE_MASK = (0xffffffff); // 2 ^ 32 - 1

class BigInteger {
public: // for now everything is public

/** The class is mimic the equivalent of base 2 but in base 2 ^ 32
 * 	
 * For now it only handle positive --> m_sign must be every time positive
 * 
**/

	int m_sign;
	std::vector<uint32_t> m_digits; // m_digits[0] is the most significant digit

	BigInteger() : m_sign(0) {}

	BigInteger(int val) {
		m_sign = 1;
		m_digits.push_back(val);

	}

	BigInteger(long long val) {
		m_sign = 1;
		if ((val >> 32) > 0) {
			m_digits.push_back(val >> 32);	
		}
		m_digits.push_back(val & BASE_MASK);
	}

	BigInteger(std::string s) {
		std::cout << "Constructor form string\n";
		m_sign = 1;
		long long sum = 0;
		int n = s.size();
		std::vector<long long> digits;
		digits.push_back(s[0] - '0');;
		for (std::size_t i = 1; i < n; i++) {

			long long carry = (s[i] - '0');

			for (std::size_t j = 0; j < digits.size(); j++) {
				digits[j] = digits[j] * 10 + carry;
				carry = 0;
				if (digits[j] >= BASE) {
					carry = digits[j] / BASE;
					digits[j] = digits[j] % BASE;
				}
			}

			if (carry > 0) {
				digits.push_back(carry);
			}
		}

		std::reverse(digits.begin(), digits.end());

		for (auto e : digits) {
			m_digits.push_back(e);
		}

		std::cout << '\n';
	}

	template <typename T>
	BigInteger(const std::vector<T>& input) {
		m_sign = 1;
		m.digits.clear();
		std::size_t i = 0;
		for (i; i < input.size(); i++) {
			if (input[i] != 0) break;
		}
		for (i; i < input.size(); i++) {
			m_digits.push_back(static_cast<uint32_t>(input[i]));
		}
	}

	void printClass() const {
		std::cout << "{m_sign = " << m_sign << ", m_digits = {";
		for (std::size_t i = 0; i < m_digits.size(); i++) {
			std::cout << m_digits[i];
			if (i + 1 < m_digits.size()) {
				std::cout << ", ";
			} 
		}
		std::cout << "}\n";
	}

	int numberOfDigits() const {
		return m_digits.size();
	}

	std::string toString() const {
		std::string result;

		result += "m_digits = ";
		int p = m_digits.size() - 1;
		for (std::size_t i = 0; i < m_digits.size(); i++) {
			result += std::to_string(m_digits[i]) + " * B ^ " + std::to_string(p);
			if (m_digits.size() > 1 && i < m_digits.size() - 1) {
				result += " + ";
			}
			p--;
		}

		std::cout << result << '\n';
		
		result = std::to_string(m_digits[0]);

		std::vector<long long> res;

		res.push_back(0);

		long long carry = 0;

		for (int i = 0; i < m_digits.size(); i++) {
			carry = m_digits[i];

			for (std::size_t j = 0; j < res.size(); j++) {
				res[j] = res[j] * BASE + carry;
				carry = res[j] / 10;
				res[j] = res[j] % 10;
			}

			while (carry > 0) {
				res.push_back(carry % 10);
				carry = carry / 10;
			}
		}

		std::reverse(res.begin(), res.end());

		result = "";

		std::cout << "result: " << '\n';
		for (auto e : res) {
			std::cout << e << " ";
			result += std::to_string(e);
		}
		std::cout << '\n';

		return result;
	}

	friend std::ostream& operator<<(std::ostream& os, const BigInteger& x) {
		return os << x.toString();
	}
};

BigInteger additionAbsolute(BigInteger& x, BigInteger y) {
	std::vector<long long> input;

	int i = x.m_digits.size() - 1;
	int j = y.m_digits.size() - 1;

	long long carry = 0;

	while (i >= 0 || j >= 0) {
		long long val = carry;
		if (i >= 0) {
			val += x.m_digits[i];
			i--;
		}
		if (j >= 0) {
			val += y.m_digits[j];
			j--;
		}

		carry = 0;

		if (val >= BASE) {
			carry = val / BASE;
			val %= BASE;
		}

		input.push_back(val);
	}
	if (carry > 0) {
		input.push_back(carry);
	}

	std::reverse(input.begin(), input.end());

	return BigInteger(input);
}

// here we suppose that x >= y
BigInteger substractionAbsolute(BigInteger& x, BigInteger& y) {
	std::vector<long long> input;

	int i = x.m_digits.size() - 1;
	int j = y.m_digits.size() - 1;

	long long borrow = 0;

	while (i >= 0 || j >= 0) {
		long long val = -borrow;
		if (i >= 0) {
			val += x.m_digits[i];
			i--;
		}
		if (j >= 0) {
			val -= y.m_digits[j];
			j--;
		}

		borrow = 0;

		if (val < 0) {
			val += BASE;
			borrow = 1; 
		}

		input.push_back(val);
	}

	std::reverse(input.begin(), input.end());

	return BigInteger(input);
}

BigInteger multiplicationAbsolute(BigInteger& x, BigInteger& y) {

    int nx = x.numberOfDigits();
    int ny = y.numberOfDigits();

    std::vector<unsigned long long> input(nx + ny, 0);

    unsigned long long carry = 0;

    for (int i = nx - 1; i >= 0; --i) {
    	std::cout << "i = " << i << '\n';
        for (int j = ny - 1; j >= 0; --j) {
            int k = i + j + 1;
			unsigned long long val = (unsigned long long)x.m_digits[i] * (unsigned long long)y.m_digits[j] + carry;
			input[k] += val;
			carry = input[k] / BASE;
            input[k] %= BASE;
        }
        input[i] += carry;
        carry = 0;
    }

	return BigInteger(input);
}

/* Knuth algorithm
 * 
 * Division of u / v, e.g. 15 / 4 = {3, 2} because 15 = 4 * 3 + 2
 * 
*/	

std::pair<BigInteger, BigInteger> divideAndRemainder(BigInteger& u, BigInteger& v) {
	std::vector<long long> quotient(u.numberOfDigits + v.numberOfDigits, 0);

	vector<uint32_t> u = u.m_digits();
	vector<uint32_t> v = v.m_digits();

	// D1 [Normalize] Ensure that v_{n - 1} > BASE / 2
	if (v.m_digits[0] < BASE / 2) {
		u = multiplication(u, BigInteger(BASE / 2));
		v = multiplication(v, BigInteger(BASE / 2));
	}

	vector<int> v_truncated = v.m_digits;
	v_truncated[0] = 0;

	// D2 [Initialization]
	int n = v.numberOfDigits();
	int m = u.numberOfDigits() - n;
	std::cout << "m = " << m << '\n';
	for (int j = 0; j < m; j++) {
		// D3 [Calculate q_hat]
		long long q_hat = (u.m_digits[j] * BASE + u.m_digits[j + 1]) / v.m_digits[0];
		long long r_hat = (u.m_digits[j] * BASE + u.m_digits[j + 1]) 
		if (q_hat >= BASE or q_hat * v.m_digits[1] >= BASE * r_hat + u.m_digits[j + 2]) {
			q_hat -= 1
			r_hat += v.m_digits[1];
		}

		// D4 [Multiply and substract]


		// D5 [Test Remainder]

		// D6 [Add Back]

		// D7 [Loop on j]
	}

	// D8 [Unormalize]


	return {BigInteger(quotient), BigInteger(remainder)}
}



using BI = BigInteger;

int main() {

	int x = 4213;
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

	//BI b8 = additionAbsolute(b6, b7);

	//std::cout << b8 << '\n';

	//BI b9 = substractionAbsolute(b7, b6);
	//std::cout << b9 << '\n';

	std::cout << "** Performing multiplication **\n";
	BI b10 = multiplicationAbsolute(b6, b7);
	std::cout << b10 << '\n';

	std::cout << "Perfom division\n";

	return 0;
}