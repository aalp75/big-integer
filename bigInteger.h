#pragma once

#include <iostream>
#include <vector>
#include <string>

class BigInteger {

// Representation: sign + magnitude in base 2^32, MSB-first.
// Invariant (recommended): zero => m_sign == 0 and m_digits empty.

//private
public: // for now let's put everything in the public scope

	int m_sign; // -1, 0, +1
	std::vector<uint32_t> m_digits; // MSB-first (Most Significant Bit)

	inline static constexpr long long SHIFT = 32;
	inline static constexpr long long BASE = 1LL << SHIFT;
	inline static constexpr long long BASE_MASK = (0xFFFFFFFF); // 2 ^ 32 - 1

public:

	// Constructors (no explicit keyword for now to be able to perform BigInteger("63") * 67)
	BigInteger();
	BigInteger(int val);
	BigInteger(long long val);
	BigInteger(unsigned long long val);
	BigInteger(std::string s);
	
	template <typename T>
	BigInteger(const std::vector<T>& input);

	// Observers & helpers
	int numberOfDigits() const;
	BigInteger abs() const;
	std::string toString() const;

	bool isNull() const;
	void printDigits() const;
	
	BigInteger(const BigInteger& other); // copy constructor
    BigInteger& operator=(const BigInteger& other); // copy assignement
    BigInteger(BigInteger&& other) noexcept; // move constructor
    BigInteger& operator=(BigInteger&& other) noexcept;// move assignement
    ~BigInteger() = default; // destructor; use by default because there is no dynamic allocation

    // Unary
    BigInteger operator+() const;
    BigInteger operator-() const;

    // Compound assigment
    BigInteger& operator+=(const BigInteger& other);
    BigInteger& operator-=(const BigInteger& other);
	BigInteger& operator*=(const BigInteger& other);
	BigInteger& operator/=(const BigInteger& other);
	BigInteger& operator%=(const BigInteger& other);



	// Increment & decrement
	BigInteger& operator++(); // prefix (++x)
	BigInteger operator++(int); // postfix (x++)
	BigInteger& operator--(); // prefix (--x)
	BigInteger operator--(int); // postfix(x--)

	// Non-member operators

	// Arithmetic operators
	friend BigInteger operator+(const BigInteger& x, const BigInteger& y);
	friend BigInteger operator-(const BigInteger& x, const BigInteger& y);
	friend BigInteger operator*(const BigInteger& x, const BigInteger& y);
	friend BigInteger operator/(const BigInteger& x, const BigInteger& y);
	friend BigInteger operator%(const BigInteger& x, const BigInteger& y);

	// Comparison operators
	friend bool operator==(const BigInteger& x, const BigInteger& y);
	friend bool operator!=(const BigInteger& x, const BigInteger& y);

	friend bool operator<(const BigInteger& x, const BigInteger& y);
	friend bool operator>(const BigInteger& x, const BigInteger& y);

	friend bool operator<=(const BigInteger& x, const BigInteger& y);
	friend bool operator>=(const BigInteger& x, const BigInteger& y);

	// Input & output
	friend std::ostream& operator<<(std::ostream& os, const BigInteger& x);
	friend std::istream& operator>>(std::istream& is, BigInteger& x);

	// Swap
	friend void swap(BigInteger& x, BigInteger& y) noexcept;
};
