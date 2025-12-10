#pragma once

#include <iostream>
#include <vector>
#include <string>

const static long long SHIFT = 32;
const static long long BASE = 1ll << SHIFT;
const static long long BASE_MASK = (0xffffffff); // 2 ^ 32 - 1

class BigInteger {
//private
public: // for now everything is public

/* The class is representing the number in base 2 ^ 32
 * 	
 * For now it only handle positive --> m_sign must be every time positive
 * 
*/

	int m_sign;
	std::vector<uint32_t> m_digits; // m_digits[0] is the most significant digit

	// constructors
	BigInteger();
	BigInteger(int val);
	BigInteger(long long val);
	BigInteger(unsigned long long val);

	BigInteger(std::string s);
	
	template <typename T>
	BigInteger(const std::vector<T>& input);

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

	// Arithmetic operators
	friend BigInteger operator+(const BigInteger& x, const BigInteger& y);
	friend BigInteger operator-(const BigInteger& x, const BigInteger& y);
	friend BigInteger operator*(const BigInteger& x, const BigInteger& y);
	friend BigInteger operator/(const BigInteger& x, const BigInteger& y);
	friend BigInteger operator%(const BigInteger& x, const BigInteger& y);

	// comparison operators
	friend bool operator==(const BigInteger& x, const BigInteger& y);
	friend bool operator!=(const BigInteger& x, const BigInteger& y);

	friend bool operator<(const BigInteger& x, const BigInteger& y);
	friend bool operator>(const BigInteger& x, const BigInteger& y);

	friend bool operator<=(const BigInteger& x, const BigInteger& y);
	friend bool operator>=(const BigInteger& x, const BigInteger& y);
};

// helpers functions

//BigInteger additionAbsolute(const BigInteger& x, const BigInteger& y);
//BigInteger substractionAbsolute(const BigInteger& x, const BigInteger& y);
//BigInteger multiplicationAbsolute(const BigInteger& x, const BigInteger& y);
//std::pair<BigInteger, BigInteger> divideAndRemainder(const BigInteger& u, const BigInteger& v);
