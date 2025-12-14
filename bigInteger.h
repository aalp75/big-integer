#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <iostream>
#include <vector>
#include <string>

class BigInteger {

// Representation: sign + words in base B (2^32), MSB-first.
// Zero is invariant: m_sign == 0 ~&& m_words empty.

private:

	int m_sign; // -1, 0, +1
	std::vector<uint32_t> m_words; // MSB-first (Most Significant Bit)

	inline static constexpr int64_t SHIFT = 32;
	inline static constexpr int64_t BASE = 1LL << SHIFT;
	inline static constexpr int64_t BASE_MASK = (0xFFFFFFFF); // 2 ^ 32 - 1
	inline static constexpr int64_t KARATSUBA_THRESHOLD = 80;

public:

	// Constructors (no explicit keyword for now to support implicit cast such that BigInteger("63") * 67)
	BigInteger();
	BigInteger(int val);
	BigInteger(unsigned int val);
	BigInteger(long val);
	BigInteger(unsigned long val);
	BigInteger(long long val);
	BigInteger(unsigned long long val);
	BigInteger(std::string s);
	
	template <typename T> BigInteger(const std::vector<T>& input);

	// Observers & helpers
	std::size_t numberOfWords() const;
	BigInteger abs() const;
	std::string toString() const;
	template<typename T> void addWord(T element);

	bool isNull() const;
	void printWords() const;
	
	BigInteger(const BigInteger& other); // copy constructor
    BigInteger& operator=(const BigInteger& other); // copy assignement
    BigInteger(BigInteger&& other) noexcept; // move constructor
    BigInteger& operator=(BigInteger&& other) noexcept;// move assignement
    ~BigInteger() = default; // destructor; use by default because there is no dynamic allocation

    // Core helper functions implementing the computation logic on absolute values
    friend BigInteger additionAbsolute(const BigInteger& x, const BigInteger& y);
    friend BigInteger substractionAbsolute(const BigInteger& x, const BigInteger& y);
    friend BigInteger multiplicationAbsolute(const BigInteger& x, const BigInteger& y);
    friend std::pair<BigInteger, BigInteger> shortDivision(const BigInteger& x, const BigInteger& y);
    friend std::pair<BigInteger, BigInteger> divideAndRemainder(const BigInteger& x, const BigInteger& y);

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

#endif
