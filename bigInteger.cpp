#include <vector>
#include <iostream>
#include <string>
#include <stdexcept> // throw runtime_error
#include <algorithm> // std::reverse and std::max
#include <cctype> // std::isdigit
#include <cstdint> // for fixed-width integer types (uint32_t & uint64_t)
#include <utility> // std::pair and std::move

#include "bigInteger.h"

//#define debug(...)

// ---------------- Constructors ----------------

BigInteger::BigInteger() : m_sign(0) {}

BigInteger::BigInteger(int val) {
	if (val == 0) { m_sign = 0; return; }
	m_sign = (val < 0) ? -1 : 1;
	m_digits.push_back(std::abs(val));
}

BigInteger::BigInteger(long long val) {
	if (val == 0) { m_sign = 0; return; }
	m_sign = (val < 0) ? -1 : 1;
	val = std::abs(val);
	if ((val >> 32) > 0) {
		m_digits.push_back(val >> 32);
	}
	m_digits.push_back(val & BASE_MASK);
}

BigInteger::BigInteger(unsigned long long val) {
	m_sign = 1;
	if ((val >> 32) > 0) {
		m_digits.push_back(val >> 32);	
	}
	m_digits.push_back(val & BASE_MASK);
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

	// clean the string
	std::string sCleaned;

	for (auto c : s) {
		if (std::isdigit(c)) sCleaned += c;
	}

	s = sCleaned;

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
}

template <typename T>
BigInteger::BigInteger(const std::vector<T>& input) {
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

// ---------------- Copy, Move & Destructor ----------------

// Copy constructors
BigInteger::BigInteger(const BigInteger& other)
	: m_sign(other.m_sign)
    , m_digits(other.m_digits) // std::vector deep-copies automatically
    {}

// Copy asignment
BigInteger& BigInteger::operator=(const BigInteger& other) {
   if (this == &other) {
		return *this;
   }
   m_sign = other.m_sign;
   m_digits = other.m_digits;
   return *this;
}

// Move constructor
BigInteger::BigInteger(BigInteger&& other) noexcept
	: m_sign(other.m_sign)
	, m_digits(std::move(other.m_digits))
	{}

// Move assignement
BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
   	if (&other == this) {
        return *this;
    }
    m_sign = other.m_sign;
	m_digits = std::move(other.m_digits);
	return *this;
}

// ---------------- Helpers & Observers ----------------

int BigInteger::numberOfDigits() const { 
	return static_cast<int>(m_digits.size());
}

std::string BigInteger::toString() const {
	if (m_sign == 0) {
		return "0";
	}

	std::vector<long long> res;

	res.push_back(0);

	long long carry = 0;

	for (std::size_t i = 0; i < m_digits.size(); i++) {
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

	std::string result;

	if (m_sign < 0) {
		result += '-';
	}

	for (auto e : res) {
		result += std::to_string(e);
	}

	return result;
}

void BigInteger::printDigits() const {
	std::cout << "[";
	for (std::size_t i = 0; i < m_digits.size(); i++) {
		std::cout << m_digits[i];
		if (i + 1 < m_digits.size()) {
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

// ---------------- Absolute arithmetic helpers ----------------

// Addition
BigInteger additionAbsolute(const BigInteger& x, const BigInteger& y) {
	int nx = x.numberOfDigits();
	int ny = y.numberOfDigits();

	int length = std::max(nx, ny);
	std::vector<unsigned long long> input(length + 1, 0);

	int xIte = nx - 1;
	int yIte = ny - 1;

	long long carry = 0;

	for (int i = length; i >= 0; i--) {
		long long val = carry;
		if (xIte >= 0) {
			val += x.m_digits[xIte];
			xIte--;
		}
		if (yIte >= 0) {
			val += y.m_digits[yIte];
			yIte--;
		}

		carry = val / BigInteger::BASE;
		input[i] = val % BigInteger::BASE;
	}
	return BigInteger(input);
}

// Substraction
// Assumes that |x| >= |y|
BigInteger substractionAbsolute(const BigInteger& x, const BigInteger& y) {
	int nx = x.numberOfDigits();
	int ny = y.numberOfDigits();

	int length = std::max(nx, ny);
	std::vector<unsigned long long> input(length, 0);

	int xIte = nx - 1;
	int yIte = ny - 1;

	long long borrow = 0;

	for (int i = length - 1; i >= 0; i--) {
		long long val = -borrow;
		if (xIte >= 0) {
			val += x.m_digits[xIte];
			xIte--;
		}
		if (yIte >= 0) {
			val -= y.m_digits[yIte];
			yIte--;
		}

		if (val < 0) {
			val += BigInteger::BASE;
			borrow = 1; 
		}
		else {
			borrow = 0;
		}
		input[i] = val;
	}

	return BigInteger(input);
}

// Multiplication
BigInteger multiplicationAbsolute(const BigInteger& x, const BigInteger& y) {
	if (x.isNull() || y.isNull()) return BigInteger(0);
    int nx = x.numberOfDigits();
    int ny = y.numberOfDigits();

    std::vector<unsigned long long> input(nx + ny, 0);

    unsigned long long carry = 0;

    for (int i = nx - 1; i >= 0; --i) {
        for (int j = ny - 1; j >= 0; --j) {
            int k = i + j + 1;
			unsigned long long val = (unsigned long long)x.m_digits[i] * (unsigned long long)y.m_digits[j] + carry;
			input[k] += val;
			carry = input[k] / BigInteger::BASE;
            input[k] %= BigInteger::BASE;
        }
        input[i] += carry;
        carry = 0;
    }

	return BigInteger(input);
}

// short divison algorithm that can be used when the number of digits of v is 1
std::pair<BigInteger, BigInteger> shortDivision(BigInteger u, BigInteger v) {
	std::vector<unsigned long long> quotient;

	unsigned long long divisor = v.m_digits[0];
	unsigned long long remainder = 0;

	for (std::size_t i = 0; i < u.numberOfDigits(); i++) {
		remainder = remainder * BigInteger::BASE + u.m_digits[i];
		unsigned long long q = remainder / divisor;
		quotient.push_back(q);
		remainder %= divisor;
	}

	return {BigInteger(quotient), BigInteger(remainder)};
}

/* Knuth Algorithm D (TAOCP Vol. 2 §4.3.1)
 * 
 * Division of u / v, e.g. 15 / 4 = {3, 2} because 15 = 4 * 3 + 2
 * 
*/	

std::pair<BigInteger, BigInteger> divideAndRemainder(const BigInteger& u_init, const BigInteger& v_init) {

	if (v_init.isNull()) {
		throw std::runtime_error("BigInteger: division by zero");
	}

	std::vector<unsigned long long> quotient;

	BigInteger u(u_init);
	BigInteger v(v_init);

	// perform int division
	if (u.numberOfDigits() == 1 && v.numberOfDigits() == 1) {

		int q = u.m_digits[0] / v.m_digits[0];
		int r =  u.m_digits[0] % v.m_digits[0];
		return {BigInteger(q), BigInteger(r)};
	}

	// perform short division
	if (v.numberOfDigits() == 1) {
		return shortDivision(u, v);
	}

	// from now on n >= 2

	int n = v.numberOfDigits();
	int m = u.numberOfDigits() - n;

	// D1 [Normalize] Ensure that v_{n - 1} > BASE / 2
	// shift the digits to the left so the MSB is set to 1

	long long d = 1ll << __builtin_clz(v.m_digits[0]);

	u = multiplicationAbsolute(u, BigInteger(d));
	v = multiplicationAbsolute(v, BigInteger(d));

	std::vector<uint32_t> currv;
	for (int i = 0; i < n - 1; i++) {
		currv.push_back(u.m_digits[i]);
	}

	int shift = 0;
	if (u.numberOfDigits() > n + m) {
		currv.push_back(u.m_digits[n - 1]);
		shift++;
	}

	BigInteger curr(currv);

	// D2 [Initialization]
	for (std::size_t j = 0; j <= m; j++) {
		curr.m_digits.push_back(u.m_digits[n - 1 + shift + j]);

		unsigned long long u0 = 0;
		unsigned long long u1 = 0;
		unsigned long long u2 = 0;

		if (curr.m_digits.size() >= n + 1) {
			u0 = curr.m_digits[0];
			u1 = curr.m_digits[1];
			u2 = curr.m_digits[2];
		}
		else if (curr.m_digits.size() >= n) {
			u1 = curr.m_digits[0];
			u2 = curr.m_digits[1];
		}

		//debug(u0, u1, u2);

		// D3 [Calculate q_hat]
		// q_hat is the initial guess. from the Theroem B: q_hat - 2 <= q <= q_hat
		unsigned long long q_hat = (u0 * BigInteger::BASE + u1) / v.m_digits[0];
		unsigned long long r_hat = (u0 * BigInteger::BASE + u1) % v.m_digits[0];

		while (q_hat >= BigInteger::BASE || q_hat * v.m_digits[1] > (BigInteger::BASE * r_hat + u2)) {
			q_hat -= 1;
			r_hat += v.m_digits[0];
			if (r_hat >= BigInteger::BASE) break;
		}


		// D4 [Multiply and substract]
		BigInteger sub = BigInteger(q_hat) * v;

		// D5 [Test Remainder]
		if (curr >= sub) {
			curr -= sub;
		}
		else { // D6 [Add Back]
			// this step is very rare, it only appends with probabably 1 / (2 ^ 31)
			// specific tests need to be written for this case
			q_hat--;
			curr += v;
			curr -= sub;
		}

		quotient.push_back(q_hat);

		// D7 [Loop on j]
	}

	// D8 [Unormalize]
	// find remainder by running shortDivision algorithm on the normalization factor d
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

	if (this->abs() >= other.abs()) {
		*this = substractionAbsolute(*this, other);
		m_sign = -other.m_sign;
	}
	else {
		*this = substractionAbsolute(other, *this);
		m_sign = other.m_sign;
	}

	if (m_digits.empty()) {
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
	return x.m_sign == y.m_sign && x.m_digits == y.m_digits;
}

bool operator!=(const BigInteger& x, const BigInteger& y) {
	return !(operator==(x, y));
}

bool operator<(const BigInteger& x, const BigInteger& y) {
   	if (x.numberOfDigits() < y.numberOfDigits()) return true;
   	if (x.numberOfDigits() > y.numberOfDigits()) return false;
   	for (int i = 0; i < x.numberOfDigits(); i++) {
       	if (x.m_digits[i] < y.m_digits[i]) return true;
        if (x.m_digits[i] > y.m_digits[i]) return false;
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
	std::swap(x.m_digits, y.m_digits);
}

