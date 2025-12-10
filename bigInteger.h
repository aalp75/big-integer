#include <iostream>
#include <vector>
#include <string>
#include "debug.h"

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

	BigInteger(unsigned long long val) {
		m_sign = 1;
		if ((val >> 32) > 0) {
			m_digits.push_back(val >> 32);	
		}
		m_digits.push_back(val & BASE_MASK);
	}

	BigInteger(std::string s) {
		//std::cout << "Constructor form string\n";
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
		m_digits.clear();
		std::size_t i = 0;
		for (i; i < input.size(); i++) {
			if (input[i] != 0) break;
		}
		for (i; i < input.size(); i++) {
			m_digits.push_back(static_cast<uint32_t>(input[i]));
		}

		if (m_digits.size() == 0) {
			m_sign = 0;
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
		if (m_sign == 0) {
			return "0";
		}
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

		//std::cout << result << '\n';
		
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

		result.clear();

		for (auto e : res) {
			result += std::to_string(e);
		}

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
BigInteger substractionAbsolute(BigInteger x, BigInteger y) {
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

BigInteger multiplicationAbsolute(BigInteger x, BigInteger y) {

    int nx = x.numberOfDigits();
    int ny = y.numberOfDigits();

    std::vector<unsigned long long> input(nx + ny, 0);

    unsigned long long carry = 0;

    for (int i = nx - 1; i >= 0; --i) {
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


// helper for the knuth algorithm
// is b1 > b2?
bool isGreaterOrEqual(BigInteger b1, BigInteger b2) {
	if (b1.m_digits.size() > b2.m_digits.size()) {
		return true;
	}
	if (b1.m_digits.size() < b2.m_digits.size()) {
		return false;
	}

	for (int i = 0; i < b1.m_digits.size(); i++) {
		if (b1.m_digits[i] > b2.m_digits[i]) {
			return true;
		}
		if (b1.m_digits[i] < b2.m_digits[i]) {
			return false;
		}
	}
	return true;
}

/* Knuth algorithm
 * 
 * Division of u / v, e.g. 15 / 4 = {3, 2} because 15 = 4 * 3 + 2
 * 
*/	

// short divison algorithm that can be used when the number of digits of v is 1

std::pair<BigInteger, BigInteger> shortDivision(BigInteger u, BigInteger v) {

	vector<long long> quotient;
	BigInteger curr(0);
	curr.m_sign = 1;
	for (int i = 0; i < u.m_digits.size(); i++) {
		curr.m_digits.push_back(u.m_digits[i]);
		if (isGreaterOrEqual(u, v)) {
			unsigned long long u0 = 0;
			unsigned long long u1 = 0;

			if (curr.m_digits.size() >= 2) {
				u0 = curr.m_digits[0];
			}
			if (curr.m_digits.size() >= 1) {
				u1 = curr.m_digits[1];
			}

			unsigned long long q = (u0 * BASE + u1) / v.m_digits[0];
			BigInteger sub = multiplicationAbsolute(BigInteger(q), v);
			curr = substractionAbsolute(curr, sub);

			quotient.push_back(q);
		}
	}

	return {BigInteger(quotient), curr};

}

std::pair<BigInteger, BigInteger> divideAndRemainder(BigInteger u, BigInteger v) {
	std::vector<unsigned long long> quotient;

	std::cout << "Initial value:\nu = " << u << "\nv = " << v << '\n';

	if (u.numberOfDigits() == 1 && v.numberOfDigits() == 1) {
		int q = u.m_digits[0] / v.m_digits[0];
		int r =  u.m_digits[0] % v.m_digits[0];
		return {BigInteger(q), BigInteger(r)};
	}

	int n = v.numberOfDigits();
	int m = u.numberOfDigits() - n;

	debug(n, m);

	debug(u.m_digits);
	debug(v.m_digits);

	// D1 [Normalize] Ensure that v_{n - 1} > BASE / 2
	long long d = 1;
	// this part can be made a bit fast because as it's a power of 2 we can know exactly what is the 
	// shift requierd
	while (v.m_digits[0] < BASE / 2) {
		d *= 2;
		u = multiplicationAbsolute(u, BigInteger(2));
		v = multiplicationAbsolute(v, BigInteger(2));
	}

	std::cout << "multiply by d = " << d << '\n';

	std::cout << "new value:\n" << "u = " << u << "\nv = " << v << '\n';

	debug(u.m_digits);
	debug(v.m_digits);

	vector<uint32_t> currv;
	for (int i = 0; i < n - 1; i++) {
		currv.push_back(u.m_digits[i]);
	}

	int shift = 0;
	if (u.numberOfDigits() > n + m) {
		currv.push_back(u.m_digits[n - 1]);
		shift++;
	}

	//return {u, v};

	BigInteger curr(currv);

	// D2 [Initialization]
	for (int j = 0; j <= m; j++) {
		std::cout << "\n\n\nj = " << j << "\n\n";
		curr.m_digits.push_back(u.m_digits[n - 1 + shift + j]);

		debug(curr.m_digits);
		std::cout << "new curr: " << curr << '\n';

		unsigned long long u0 = 0;
		unsigned long long u1 = 0;

		debug(curr.m_digits.size());

		if (curr.m_digits.size() >= n + 1) {
			u0 = curr.m_digits[0];
		}
		if (curr.m_digits.size() >= n) {
			u1 = curr.m_digits[1];
		}

		debug(u0, u1);

		// D3 [Calculate q_hat]
		// q_hat is the initial guess. from the Theroem B: q_hat - 2 <= q <= q_hat
		unsigned long long q_hat = (u0 * BASE + u1) / v.m_digits[0];
		unsigned long long r_hat = (u0 * BASE + u1) % v.m_digits[0];

		std::cout << "q_hat = " << q_hat << '\n';
		std::cout << "r_hat = " << r_hat << '\n';

		long long u2 = 0;
		if (curr.m_digits.size() >= 3 && curr.m_digits.size() >= n - 1) {
			u2 = curr.m_digits[2];
		}

		while (q_hat >= BASE || q_hat * v.m_digits[1] >= (BASE * r_hat + u2)) {
			std::cout << "do an adjust\n";
			q_hat -= 1;
			r_hat += v.m_digits[0];
			if (r_hat >= BASE) break;
		}


		std::cout << "After adjustment: \n";
		std::cout << "q_hat = " << q_hat << '\n';
		std::cout << "r_hat = " << r_hat << '\n';

		// D4 [Multiply and substract]
		BigInteger sub = multiplicationAbsolute(BigInteger(q_hat), v);
		std::cout << "sub : " << sub << '\n';

		debug(curr.m_digits);
		debug(sub.m_digits);

		std::cout << "curr : " << curr << '\n';

		debug(isGreaterOrEqual(curr, sub));

		// D5 [Test Remainder]
		if (isGreaterOrEqual(curr, sub)) {
			std::cout << "It's greater so I can multiply\n";
			std::cout << curr << " - " << sub << " = " << substractionAbsolute(curr, sub) << '\n';
			curr = substractionAbsolute(curr, sub);
			std::cout << "curr : " << curr << '\n';
			debug(curr.m_digits);
			
		}
		else { // D6 [Add Back]
			// this step is very rare, it only appends with probabably 1 / (2 ^ 31)
			std::cout << "PROBLEM!! It becomes negative so I need to handle carefully\n";
			q_hat--;
			curr = additionAbsolute(curr, v); // curr += v
			curr = substractionAbsolute(curr, sub); // curr -= sub
		}

		quotient.push_back(q_hat);

		// D7 [Loop on j]
	}

	// D8 [Unormalize]
	debug(quotient);

	BigInteger remainder(curr);
	debug(d);
	std::cout << "curr before = " << curr << '\n';
	if (d != 1) {
		auto res = shortDivision(curr, BigInteger(d));
		remainder = BigInteger(res.first);
	}

	return {BigInteger(quotient), remainder};
}

using BI = BigInteger;
