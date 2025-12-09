#include <iostream>
#include <string>
#include <vector>
#include <chrono>

// Summary
// for now only consider positive number

class BigIntegerString {
//private: for now everything is public
public:
	int m_sign;
	std::string m_digits;

	std::string toString() const 
	{
		std::string res;
		if (m_sign == 0)
		{
			return "0";
		}
		if (m_sign < 0) 
		{
			res = "-";
		}
		res += m_digits;
		return res;
	}

	void clean() {
		// remove all leading zeros
		std::string res = "";
		bool nonZero = false;
		for (auto c : m_digits) {
			if (c == '-') continue;
			if (nonZero || c != '0') {
				res += c;
				nonZero = true;
				continue;
			}
		}
		if (res.empty()) {
			res = "0";
		}
		m_digits = res;
	}

	bool isValid() 
	{
		//TO DO: check if the input string is valid
		// if it's not throw an error
		return true;
	}

//public:
	BigIntegerString() : m_sign(0), m_digits("0") {}

	BigIntegerString(int value)
	{
		
		if (value == 0) {
			m_sign = 0;
			m_digits = "0";
			return;
		}

		m_digits = std::to_string(value);

		if (value > 0) {
			m_sign = 1;
		}
		else {
			m_sign = 0;
		}
	}

	BigIntegerString(std::string value)
	{
		m_sign = 1;
		for (auto c : value) {
			if (c == '-') {
				m_sign = -1;
			}
			else {
				m_digits += c;
			}
		}

		clean();
		if (m_digits == "0") {
			m_sign = 0;
		}
	}

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const BigIntegerString& x) 
    {
        return os << x.toString();
    }
};

using BIG = BigIntegerString;

// is x < y
bool compareAbsolute(const BigIntegerString &x, const BigIntegerString& y) {
	if (x.m_digits.size() < y.m_digits.size()) {
		return true;
	}
	if (x.m_digits.size() > y.m_digits.size()) {
		return false;
	}

	// same length
	for (int i = 0; i < x.m_digits.size(); i++) {
		if (x.m_digits[i] < y.m_digits[i]) return true;
		if (x.m_digits[i] > y.m_digits[i]) return false;
	}

	// if equal return false
	return false;
}

BigIntegerString additionAbsolute(const BigIntegerString& x, const BigIntegerString& y) 
{
	int i = x.m_digits.size() - 1;
	int j = y.m_digits.size() - 1;

	std::string res;

	int carry = 0;

	while (i >= 0 or j >= 0) 
	{
		int v = carry;
		if (i >= 0) {
			v += x.m_digits[i] - '0';
		}
		if (j >= 0) {
			v += y.m_digits[j] - '0';
		}
		carry = 0;
		if (v >= 10) {
			v -= 10;
			carry = 1;
		}
		res += v + '0';
		i--;
		j--;
	}

	if (carry) {
		res += "1";
	} 
	reverse(res.begin(), res.end());
	return BigIntegerString(res);
}


// suppose that |x| > |y| and both are non negative
BigIntegerString substractionAbsolute(const BigIntegerString& x, const BigIntegerString& y) 
{

	int i = x.m_digits.size() - 1;
	int j = y.m_digits.size() - 1;

	std::string result;

	int borrow = 0;

	while (i >= 0)
	{
		int v = (x.m_digits[i] - '0') - borrow;
		if (j >= 0) 
		{
			v -= y.m_digits[j] - '0';
		}
		borrow = 0;
		if (v < 0)
		{
			v += 10;
			borrow = 1;
		}
		i--; j--;
		result += v + '0';
	}	

	std::reverse(result.begin(), result.end());
	return BigIntegerString(result);
}

/* school multiplication 
*/

BigIntegerString
multiplication(const BigIntegerString& x, const BigIntegerString& y) 
{


	if (x.m_sign == 0 || y.m_sign == 0) 
	{
		return BigIntegerString();
	}

	int sign = x.m_sign * y.m_sign;
	
	int n = x.m_digits.size();
	int m = y.m_digits.size();

	std::vector<int> partial(n + m, 0);

	for (int i = n - 1; i >= 0; i--) 
	{
		int v1 = x.m_digits[i] - '0';
		for (int j = m - 1; j >= 0; j--) {
			int v2 = y.m_digits[j] - '0';
			partial[i + j + 1] += v1 * v2; 
		}
	}

	std::string result;
	int carry = 0;

	for (int i = n + m - 1; i >= 0; i--)
	{
		int v = carry;
		v += partial[i];
		carry = v / 10;
		v %= 10;
		result += v + '0';
	}
	if (carry) 
	{
		result += carry + '0';
	}

	std::reverse(result.begin(), result.end());

	return BigIntegerString(result);
}

/** 
  * short division algorithm
  *
  * works well but it's very inefficient on big number
**/

std::pair<BigIntegerString, BigIntegerString> 
divideAndRemainder(const BigIntegerString& x, const BigIntegerString& y) 
{
	if (y.m_sign == 0) {
		throw std::runtime_error("Error: dividing by 0");
	}

	int n = x.m_digits.size();

	std::string quotient;
	std::string remainder;

	for (int i = 0; i < n; i++) {

		remainder += x.m_digits[i];
		BigIntegerString curr(remainder);

		int best = 0;
		for (int d = 1; d <= 9; d++) {
			BigIntegerString dBig(d);
			BigIntegerString prod = multiplication(y, dBig);
	
			// if prod <= curr
			if (!compareAbsolute(curr, prod)) {
				best = d;
			}
			else {
				break;
			}
		}

		quotient += best + '0';

		if (best > 0) {
			BigIntegerString bestBig(best);
			BigIntegerString prod = multiplication(y, bestBig);
			BigIntegerString newCurr = substractionAbsolute(curr, prod);
			remainder = newCurr.m_digits;
		}
	}
	return {BigIntegerString(quotient), BigIntegerString(remainder)};
}

std::pair<BigIntegerString, BigIntegerString>
divideAndRemainderKnuth(BigIntegerString& u, BigIntegerString& v)
{

	std::string quotient;
	std::string remainder;


	return {BigIntegerString(quotient), BigIntegerString(remainder)};
}

void timing() 
{

	using clock = std::chrono::high_resolution_clock;

    auto start = clock::now();
    for (int i = 0; i < 5; i++)
    {
			BIG b5("12421446");
			BIG b6("232344");

			BIG b7 = multiplication(b5, b6);
    }
    auto end = clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << diff.count() << " seconds\n";
}

int main()
{

	BIG b1("233");
	BIG b2("14");

	BIG b3 = additionAbsolute(b1, b2);

	std::cout << b3 << '\n';
	std::cout << 1234213 + 2534 << '\n';



	BIG b4 = multiplication(b1, b2);
	std::cout << b4 << '\n';
	std::cout << 234 * 14 << '\n';

	BIG b5("12421446");
	BIG b6("232344");

	BIG b7 = multiplication(b5, b6);
	std::cout << b7 << " (" << 1ll * 12421446 * 232344 << ")\n";
	//timing();

	BIG b8("1238");
	BIG b9("99");


	BIG b10 = substractionAbsolute(b8, b9);
	std::cout << b10 << " (" << 1238 - 99 << ")\n";

	BIG b11("5243");
	BIG b12("42");

	std::pair<BIG, BIG> b13 = divideAndRemainder(b11, b12);

	std::cout << "q = " << b13.first << " r = " << b13.second << '\n';
	std::cout << "check : q = " << 5243 / 42 << " r  = " << 5243 % 42 << '\n';

	return 0;
}