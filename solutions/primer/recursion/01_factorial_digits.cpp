// Problem: Four tiny recursive functions. Use recursion, not loops.
//   factorial(n)         n! for n >= 0 (0! = 1)
//   sumDigits(n)         sum of the decimal digits of n >= 0
//   power(base, exp)     base raised to exp >= 0 (anything^0 = 1)
//   reverseString(s)     s reversed
//
// Examples:
//   factorial(5)         -> 120
//   sumDigits(1234)      -> 10
//   power(2, 10)         -> 1024
//   reverseString("abc") -> "cba"
//
// Edge cases: factorial(0), sumDigits(0), power(x, 0), empty and one-char strings.
//
// Run with: make run FILE=practice/cpp/recursion/01_factorial_digits.cpp
//
// Syntax reminder:
//   long long f(int n) { if (n <= 1) return 1;  return n * f(n - 1); }   // base case first
//   n % 10   // last digit        n / 10   // drop last digit
//   s.substr(1)                   // everything but the first char
//   s.empty(), s.size(), s[0], s.back()
//   long long: use for results that can exceed int (factorial(15) does)

#include <string>

#include "check.h"

long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int sumDigits(int n) {
    if (n < 10) return n;
    return n % 10 + sumDigits(n / 10);
}

long long power(long long base, int exp) {
    if (exp == 0) return 1;
    return base * power(base, exp - 1);
}

std::string reverseString(const std::string& s) {
    if (s.size() <= 1) return s;
    return reverseString(s.substr(1)) + s[0];
}

int main() {
    CHECK_EQ(factorial(0), 1LL);
    CHECK_EQ(factorial(1), 1LL);
    CHECK_EQ(factorial(5), 120LL);
    CHECK_EQ(factorial(15), 1307674368000LL);

    CHECK_EQ(sumDigits(0), 0);
    CHECK_EQ(sumDigits(7), 7);
    CHECK_EQ(sumDigits(1234), 10);
    CHECK_EQ(sumDigits(1000), 1);

    CHECK_EQ(power(2, 0), 1LL);
    CHECK_EQ(power(2, 10), 1024LL);
    CHECK_EQ(power(0, 0), 1LL);
    CHECK_EQ(power(-3, 3), -27LL);

    CHECK_EQ(reverseString(""), "");
    CHECK_EQ(reverseString("a"), "a");
    CHECK_EQ(reverseString("abc"), "cba");

    return check::report();
}
