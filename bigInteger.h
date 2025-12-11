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

	int numberOfDigits() const { return m_digits.size();}

	std::string toString() const;
	
	BigInteger(const BigInteger& other); // copy constructor
    BigInteger& operator=(const BigInteger& other); // copy assignement
    BigInteger(BigInteger&& other) noexcept; // move constructor
    BigInteger& operator=(BigInteger&& other) noexcept;// move assignement

    // delete, use by default because nothing is dynamically allocated
    ~BigInteger() = default;



	BigInteger addAbsolute(const BigInteger& other) const;

	BigInteger& operator+=(const BigInteger& other);

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
