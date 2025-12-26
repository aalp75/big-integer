#include <vector>
#include <iostream>
#include <string>
#include <stdexcept> // throw runtime_error
#include <algorithm> // std::reverse and std::max
#include <cctype> // std::isdigit
#include <cstdint> // for fixed-width integer types (uint32_t & uint64_t)
#include <utility> // std::pair and std::move
#include <cstdlib> // std::abs
#include <bit> // std::countl_zero

#include "bigInteger.h"

// ---------------- Constructors ----------------

BigInteger::BigInteger() : m_sign(0) {}

BigInteger::BigInteger(int val) {
	if (val == 0) { m_sign = 0; return; }
	m_sign = (val < 0) ? -1 : 1;
	addWord(std::abs(val));
}

BigInteger::BigInteger(unsigned int val) {
	if (val == 0) { m_sign = 0; return; }
	m_sign = 1;
	addWord(val);
}

BigInteger::BigInteger(long val) {
	if (val == 0) { m_sign = 0; return; }
	m_sign = (val < 0) ? -1 : 1;
	if (val < 0) val = -val;
	if ((val >> 32) > 0) {
		addWord(val >> 32);
	}
	addWord(static_cast<uint64_t>(val) & BigInteger::BASE_MASK);
}

BigInteger::BigInteger(unsigned long val) {
	if (val == 0) { m_sign = 0; return; }
	m_sign = 1;
	if ((val >> 32) > 0) {
		addWord(val >> 32);
	}
	m_words.push_back(static_cast<uint32_t>(val & BASE_MASK));
}

BigInteger::BigInteger(long long val) {
	if (val == 0) { m_sign = 0; return; }
	m_sign = (val < 0) ? -1 : 1;
	if (val < 0) val = std::abs(val);
	if ((val >> 32) > 0) {
		addWord(val >> 32);
	}
	addWord(static_cast<uint64_t>(val) & BASE_MASK);
}

BigInteger::BigInteger(unsigned long long val) {
	if (val == 0) { m_sign = 0; return; }
	if ((val >> 32) > 0) {
		m_words.push_back(static_cast<uint32_t>(val >> 32));
	}
	m_words.push_back(static_cast<uint32_t>(val & BASE_MASK));
}

BigInteger::BigInteger(const std::vector<uint32_t>& input) {
	m_sign = 1;
	m_words.clear();
	std::size_t i = 0;
	for (; i < input.size(); i++) {
		if (input[i] != 0) break;
	}
	for (; i < input.size(); i++) {
		m_words.push_back(static_cast<uint32_t>(input[i]));
	}

	if (m_words.size() == 0) {
		m_sign = 0;
	}
}

BigInteger::BigInteger(std::string s) {
	if (s.empty()) { // TODO: use an isValid method
		throw std::invalid_argument("BigInteger: empty string");
	}

	if (s == "0") {
		m_sign = 0;
		return;
	}

	if (s[0] == '-') {
		m_sign = -1;
	}
	else {
		m_sign = 1;
	}

	std::string sCleaned;

	for (char c : s) {
		if (std::isdigit(c)) sCleaned += c;
	}

	s = sCleaned;

	if (s.empty()) {
		throw std::invalid_argument("BigInteger: empty string");
	}
	
	if (s == "0") {
		m_sign = 0;
		return;
	}

	std::size_t n = s.size();
	std::vector<uint32_t> digits;
	digits.push_back(static_cast<uint32_t>(s[0] - '0'));
	for (std::size_t i = 1; i < n; i++) {

		uint64_t carry = static_cast<uint64_t>(s[i] - '0');

		for (std::size_t j = 0; j < digits.size(); j++) {
			uint64_t val = digits[j];
			val = val * 10 + carry;
			//digits[j] = digits[j] * 10 + carry;
			carry = 0;
			if (val >= BASE) {
				carry = val / BASE;
				digits[j] = static_cast<uint32_t>(val % BASE);
			}
			else {
				carry = 0;
				digits[j] = static_cast<uint32_t>(val);
			}
		}

		if (carry > 0) {
			digits.push_back(static_cast<uint32_t>(carry));
		}
	}

	std::reverse(digits.begin(), digits.end());
	m_words = digits;
}

// ---------------- Copy, Move & Destructor ----------------

// Copy constructor
BigInteger::BigInteger(const BigInteger& other)
	: m_sign(other.m_sign)
    , m_words(other.m_words) // std::vector deep-copies automatically
    {}

// Copy assignment
BigInteger& BigInteger::operator=(const BigInteger& other) {
   if (this == &other) {
		return *this;
   }
   m_sign = other.m_sign;
   m_words = other.m_words;
   return *this;
}

// Move constructor
BigInteger::BigInteger(BigInteger&& other) noexcept
	: m_sign(other.m_sign)
	, m_words(std::move(other.m_words))
	{}

// Move assignement
BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
   	if (&other == this) {
        return *this;
    }
    m_sign = other.m_sign;
	m_words = std::move(other.m_words);
	return *this;
}

// ---------------- Helpers & Observers ----------------

std::size_t BigInteger::numberOfWords() const { 
	return m_words.size();
}

std::string BigInteger::toString() const {
	if (m_sign == 0) {
		return "0";
	}

	std::vector<uint64_t> res;

	res.push_back(0);

	uint64_t carry = 0;

	for (std::size_t i = 0; i < m_words.size(); i++) {
		carry = m_words[i];

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

	std::string result;

	if (m_sign < 0) {
		result += '-';
	}

	for (auto e : res) {
		result += std::to_string(e);
	}

	return result;
}

void BigInteger::printWords() const {
	std::cout << "[";
	for (std::size_t i = 0; i < m_words.size(); i++) {
		std::cout << m_words[i];
		if (i + 1 < m_words.size()) {
			std::cout << ", ";
		}
	}
	std::cout << "]\n";
}

BigInteger BigInteger::abs() const {
	BigInteger res(*this);
	if (res.m_sign < 0) res.m_sign = 1;
	return res;
}

bool BigInteger::isNull() const {
	return m_sign == 0;
}

// ---------------- Absolute Arithmetic Helpers ----------------

// Addition, subtraction, Multiplication, Division

/** 
 * Addition on |x| + |y|
 * 
 * Perform the schoolbook addition:
 * - loop on each digits
 * - add them 
 * - roll over a carry
 * 
 * Return: BigInteger result of |x| + |y|
 * 
 * Complexity = O(max(x.numberOfWords(), y.numberOfWords()))
 *  
 */

BigInteger additionAbsolute(const BigInteger& x, const BigInteger& y) {
	std::size_t nx = x.numberOfWords();
	std::size_t ny = y.numberOfWords();

	std::size_t length = std::max(nx, ny);
	std::vector<uint32_t> input(length + 1, 0);

	std::size_t xIte = nx;
	std::size_t yIte = ny;

	uint64_t carry = 0;

	for (std::size_t i = 0; i <= length; i++) {
		uint64_t val = carry;
		if (xIte > 0) val += x.m_words[--xIte];
		if (yIte > 0) val += y.m_words[--yIte];

		input[length - i] = val % BigInteger::BASE;
		carry = val / BigInteger::BASE;
	}

	return BigInteger(input);
}

/** 
 * subtraction on |x| - |y|
 * 
 * Condition: |x| > |y|
 * 
 * Perform the schoolbook subtraction:
 * - loop on each digits
 * - substract y[i] from x[i] 
 * - roll over a borrow if the result of the above step is negative
 * 
 * Return: BigInteger result of |x| - |y|
 * 
 * Complexity = O(max(x.numberOfWords(), y.numberOfWords()))
 */
BigInteger subtractionAbsolute(const BigInteger& x, const BigInteger& y) {
	std::size_t nx = x.numberOfWords();
	std::size_t ny = y.numberOfWords();

	std::size_t length = (std::max(nx, ny));
	std::vector<uint32_t> input(length, 0);

	std::size_t xIte = nx;
	std::size_t yIte = ny;

	int64_t borrow = 0;

	for (std::size_t i = 0; i < length; i++) {
		int64_t val = -borrow;
		if (xIte > 0) {
			val += x.m_words[--xIte];
		} 
		if (yIte > 0) {
			val -= y.m_words[--yIte];
		}
		borrow = 0;
		if (val < 0) {
			val += BigInteger::BASE;
			borrow = 1; 
		}
		input[length - 1 - i] = static_cast<uint32_t>(val);
	}

	return BigInteger(input);
}

/** 
 * Multiplication on |x| * |y|
 *  
 * Perform the schoolbook multiplication:
 * - loop on each digits of x
 * - loop on each digits of y
 * - perform the multiplication of each pair of digits
 * - roll over a carry
 * 
 * Return: BigInteger result of |x| * |y|
 * 
 * Complexity = O(x.numberOfWords() * y.numberOfWords())
 */
BigInteger multiplicationAbsolute(const BigInteger& x, const BigInteger& y) {
	if (x.isNull() || y.isNull()) return BigInteger(0);

    std::size_t nx = x.numberOfWords();
    std::size_t ny = y.numberOfWords();

    if (nx >= BigInteger::KARATSUBA_THRESHOLD && ny >= BigInteger::KARATSUBA_THRESHOLD) {
    	return karatsubaMultiplication(x, y);
    }

    std::vector<uint32_t> input(nx + ny, 0);

    uint64_t carry = 0;

    for (std::size_t i = 0; i < nx; i++) {
    	for (std::size_t j = 0; j < ny; j++) {

    		std::size_t k = nx + ny - 1 - i - j;

    		uint64_t val = (uint64_t)x.m_words[nx - 1 - i] * (uint64_t)y.m_words[ny - 1 - j];
    		val += carry;
    		val += input[k];

    		input[k] = static_cast<uint32_t>(val % BigInteger::BASE);
			carry = val / BigInteger::BASE;
    	}
        input[nx - 1 - i] = static_cast<uint32_t>(carry);
        carry = 0;
    }

	return BigInteger(input);
}

/** 
 * Multiplication on |x| * |y|
 * 
 * Returns a BigInteger
 * 
 * Multiplication is based on karatsuba algorithm:
 * - x = xHigh * B ^ middle + xLow
 * - y = yHigh * B ^ middle + yHigh
 * - x * y = z2 * B ^ (2 * middle) + z1 + B ^ middle + z0
 * - z2 = xHigh * yHigh, z0 = xLow * yHigh ans z1 = (xLow + xHigh) * (yLow + yHigh) - z2 - z0
 * - 3 small multiplications instead of 4 for classical schoolbook multplication
 * 
 *  * Return: BigInteger result of |x| * |y|
 * 
 * Complexity = O(max(x.numberOfWords(), y.numberOfWords() ) ^ log2(3)) ~ n ^ 1.59
 *  
 */

BigInteger karatsubaMultiplication(const BigInteger& x, const BigInteger& y) {

	std::size_t nx = x.numberOfWords();
	std::size_t ny = y.numberOfWords();

	if (std::min(nx, ny) <= BigInteger::KARATSUBA_THRESHOLD) {
		return multiplicationAbsolute(x, y);
	}

	std::size_t n = std::max(nx, ny);
	std::size_t middle = n / 2;

	// split x and y
	std::vector<uint32_t> wordsXLow, wordsXHigh, wordsYLow, wordsYHigh;

	for (std::size_t i = 0; i < nx; i++) {
		if (i + middle < nx) {
			wordsXHigh.push_back(x.m_words[i]);
		}
		else {
			wordsXLow.push_back(x.m_words[i]);
		}
	}

	for (std::size_t i = 0; i < ny; i++) {
		if (i + middle < ny) {
			wordsYHigh.push_back(y.m_words[i]);	
		}
		else {
			wordsYLow.push_back(y.m_words[i]);
		}
	}

	BigInteger xLow(wordsXLow), xHigh(wordsXHigh), yLow(wordsYLow), yHigh(wordsYHigh);

	// recursive multiplications
	BigInteger z0 = karatsubaMultiplication(xLow, yLow);
	BigInteger z2 = karatsubaMultiplication(xHigh, yHigh);

    BigInteger sumX = xLow + xHigh;
    BigInteger sumY = yLow + yHigh;

	BigInteger z3 = karatsubaMultiplication(sumX, sumY);

	BigInteger z1 = z3 - z2 - z0;

	// shift and combine the reults
	z1.m_words.insert(z1.m_words.end(), middle, 0);
	z2.m_words.insert(z2.m_words.end(), 2 * middle, 0);

	BigInteger res = z2 + z1 + z0;

	return res;
}

/** 
 * Division on |x| / |y|
 * 
 * Condition: y has only 1 digit; i.e. it's less or equal than BASE (2^32)
 *  
 * Perform the short division algorithm:
 * - loop on each digits of x
 * - perform the long division when it's able and store the result
 * 
 * Return: pair<BigInteger, BigInteger> = {quotient, remainder}
 * 
 * Complexity = O(u.numberOfWords()) 
 * 
 */

std::pair<BigInteger, BigInteger> shortDivision(const BigInteger& x, const BigInteger& y) {
	std::vector<uint32_t> quotient;

	uint64_t divisor = y.m_words[0];
	uint64_t remainder = 0;

	for (std::size_t i = 0; i < x.numberOfWords(); i++) {
		remainder = remainder * BigInteger::BASE + x.m_words[i];
		uint64_t q = remainder / divisor;
		quotient.push_back(static_cast<uint32_t>(q));
		remainder %= divisor;
	}

	return {BigInteger(quotient), BigInteger(remainder)};
}

/** 
 * Knuth Division Algorithm D Division (from TAOCP Volume 2 4.3.1) 
 * 
 * Condition: y != 0
 * 
 * Return: pair<BigInteger, BigInteger> = {quotient, remainder}
 * 
 * Complexity = O(x.numberOfWords() * y.numberOfWords())
 *  
*/

std::pair<BigInteger, BigInteger> divideAndRemainder(const BigInteger& x, const BigInteger& y) {

	if (y.isNull()) {
		throw std::runtime_error("BigInteger: division by zero");
	}

	std::vector<uint32_t> quotient;

	// perform integer division
	if (x.numberOfWords() == 1 && y.numberOfWords() == 1) {

		uint64_t q = x.m_words[0] / y.m_words[0];
		uint64_t r = x.m_words[0] % y.m_words[0];
		return {BigInteger(q), BigInteger(r)};
	}

	// perform short division algorithm
	if (y.numberOfWords() == 1) {
		return shortDivision(x, y);
	}

	// from now on both numbers have at least 2 words length

	BigInteger u(x);
	BigInteger v(y);

	std::size_t n = v.numberOfWords();
	std::size_t m = u.numberOfWords() - n;

	// D1 [Normalize] Ensure that v_{n - 1} > BASE / 2
	// shift the digits to the left so the most significant bit of the 1-st word is set to 1

	uint32_t d = 1ll << std::countl_zero(v.m_words[0]);

	u = multiplicationAbsolute(u, BigInteger(d));
	v = multiplicationAbsolute(v, BigInteger(d));

	// TODO: enhance this logic
	std::vector<uint32_t> currv;
	for (std::size_t i = 0; i < n - 1; i++) {
		currv.push_back(u.m_words[i]);
	}

	std::size_t shift = 0;
	if (u.numberOfWords() > n + m) {
		currv.push_back(u.m_words[n - 1]);
		shift++;
	}

	BigInteger curr(currv);

	// D2 [Initialization]
	for (std::size_t j = 0; j <= m; j++) {
		curr.addWord(u.m_words[n - 1 + shift + j]);

		// TODO: enhance the computation of u0, u1 and u2
		uint64_t u0 = 0;
		uint64_t u1 = 0;
		uint64_t u2 = 0;

		if (curr.numberOfWords() >= n + 1) {
			u0 = curr.m_words[0];
			u1 = curr.m_words[1];
			u2 = curr.m_words[2];
		}
		else if (curr.numberOfWords() >= n) {
			u1 = curr.m_words[0];
			u2 = curr.m_words[1];
		}

		// D3 [Calculate q_hat]
		// q_hat is the initial guess; from the Theorem B we know that q_hat - 2 <= q <= q_hat
		uint64_t q_hat = (u0 * BigInteger::BASE + u1) / v.m_words[0];
		uint64_t r_hat = (u0 * BigInteger::BASE + u1) % v.m_words[0];

		while (q_hat >= BigInteger::BASE || q_hat * v.m_words[1] > (BigInteger::BASE * r_hat + u2)) {
			q_hat -= 1;
			r_hat += v.m_words[0];
			if (r_hat >= BigInteger::BASE) break;
		}

		// D4 [Multiply and substract]
		BigInteger sub = BigInteger(q_hat) * v;

		// D5 [Test Remainder]
		if (curr >= sub) {
			curr -= sub;
		}
		else { // D6 [Add Back]
			// this step is very rare, it only happends with probability 1 / (2 ^ 31)
			// specific tests are written for this step
			std::cout << "D6 [Add Back]\n";
			q_hat--;
			curr += v;
			curr -= sub;
		}

		quotient.push_back(static_cast<uint32_t>(q_hat));

	} // D7 [Loop on j]

	// D8 [Unormalize]
	// find the remainder by running shortDivision algorithm on the normalization factor d
	BigInteger remainder(curr);

	std::pair<BigInteger, BigInteger> res = shortDivision(curr, BigInteger(d));
	remainder = BigInteger(res.first);

	return {BigInteger(quotient), remainder};
}

// ---------------- Operators ----------------

BigInteger BigInteger::operator+() const {
	return *this;
}
BigInteger BigInteger::operator-() const {
	BigInteger res(*this);
	res.m_sign *= -1;
	return res;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
	if (other.m_sign == 0) return *this;
	if (m_sign == 0) {
		*this = other;
		return *this;
	}

	if (m_sign == other.m_sign) {
		*this = additionAbsolute(*this, other);
		m_sign = other.m_sign;
		return *this;
	}

	// signs are different

	if (this->abs() >= other.abs()) {
		*this = subtractionAbsolute(*this, other);
		m_sign = -other.m_sign;
	}
	else {
		*this = subtractionAbsolute(other, *this);
		m_sign = other.m_sign;
	}

	if (m_words.empty()) {
		m_sign = 0;
	}

	return *this;
}

BigInteger& BigInteger::operator++() { //  prefix (++x)
	*this += BigInteger(1);
	return *this;
}

BigInteger BigInteger::operator++(int) { // postfix (x++)
	BigInteger old = *this;
	++(*this);
	return old;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
	if (other.isNull()) return *this;
    *this += (-other);
    return *this;
}

BigInteger& BigInteger::operator--() {// prefix (--x)
	*this -= BigInteger(1);
	return *this;
}

BigInteger BigInteger::operator--(int) { // postfix(x--)
	BigInteger old = *this;
	--(*this);
	return old;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
	int finalSign = m_sign * other.m_sign;
	*this = multiplicationAbsolute(*this, other);
	m_sign = finalSign;
	return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
	int finalSign = m_sign * other.m_sign;
	*this = divideAndRemainder(*this, other).first;
	m_sign = finalSign;
	return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
	*this = divideAndRemainder(*this, other).second;
	return *this;
}

BigInteger operator+(const BigInteger& x, const BigInteger& y) {
	return BigInteger(x) += y;
}

BigInteger operator-(const BigInteger& x, const BigInteger& y) {
	return BigInteger(x) -= y;
}

BigInteger operator*(const BigInteger& x, const BigInteger& y) {
	return BigInteger(x) *= y;
}

BigInteger operator/(const BigInteger& x, const BigInteger& y) {
	return BigInteger(x) /= y;
}

BigInteger operator%(const BigInteger& x, const BigInteger& y) {
	return BigInteger(x) %= y;
}

// comparison operators
bool operator==(const BigInteger& x, const BigInteger& y) {
	return x.m_sign == y.m_sign && x.m_words == y.m_words;
}

bool operator!=(const BigInteger& x, const BigInteger& y) {
	return !(operator==(x, y));
}

bool operator<(const BigInteger& x, const BigInteger& y) {

	if (x.isNull() && y.isNull()) return false;

	if (x.m_sign < y.m_sign) return true;
	if (x.m_sign > y.m_sign) return false;

	if (x.m_sign == 1) {
	   	if (x.numberOfWords() < y.numberOfWords()) return true;
	   	if (x.numberOfWords() > y.numberOfWords()) return false;
	   	for (std::size_t i = 0; i < x.numberOfWords(); i++) {
	       	if (x.m_words[i] < y.m_words[i]) return true;
	        if (x.m_words[i] > y.m_words[i]) return false;
	   	}
   	}
   	else {
	   	if (x.numberOfWords() < y.numberOfWords()) return false;
	   	if (x.numberOfWords() > y.numberOfWords()) return true;
	   	for (std::size_t i = 0; i < x.numberOfWords(); i++) {
	       	if (x.m_words[i] < y.m_words[i]) return false;
	        if (x.m_words[i] > y.m_words[i]) return true;
	   	}		
   	}
   	return false;
}

bool operator>(const BigInteger& x, const BigInteger& y) {
   return operator<(y, x);
}

bool operator<=(const BigInteger& x, const BigInteger& y) {
    return !operator>(x, y);
}

bool operator>=(const BigInteger& x, const BigInteger& y) {
   	return !operator<(x, y);
}

std::ostream& operator<<(std::ostream& os, const BigInteger& x) {
	return os << x.toString();
}

std::istream& operator>>(std::istream& is, BigInteger& x) {
	std::string s;
	if (!(is >> s)) return is;
	x = BigInteger(s);
	return is;
}

void swap(BigInteger& x, BigInteger& y) noexcept {
	std::swap(x.m_sign, y.m_sign);
	std::swap(x.m_words, y.m_words);
}