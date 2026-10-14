/*
 * Problem: Loops - digits, primes, sequences
 *   Practice while/for loops with a few classic small tasks.
 *
 *     digitSum(n)          Sum of the decimal digits of |n|.
 *     reverseNumber(n)     Reverse the decimal digits of n, keeping the sign.
 *                          Trailing zeros vanish (120 -> 21). If the reversed value
 *                          does not fit in int, return 0.
 *     isPrime(n)           True iff n is prime (n < 2 is not prime).
 *     countPrimesUpTo(n)   How many primes p satisfy 2 <= p <= n (0 if n < 2).
 *     collatzSteps(n)      For n >= 1: repeat "if even n /= 2, else n = 3n + 1" and
 *                          count the steps until n reaches 1.
 *
 * Examples:
 *   digitSum(-4091)     -> 14
 *   reverseNumber(-120) -> -21
 *   isPrime(97)         -> true
 *   countPrimesUpTo(10) -> 4      (2, 3, 5, 7)
 *   collatzSteps(6)     -> 8      (6 3 10 5 16 8 4 2 1)
 *
 * Constraints / edge cases:
 *   - digitSum(0) == 0; digitSum(INT_MIN) works (|INT_MIN| does not fit in int, so
 *     widen to long long first).
 *   - reverseNumber: INT_MAX and INT_MIN reverse to something too large -> 0. Check for
 *     overflow BEFORE multiplying by 10 (or accumulate in long long and compare).
 *   - isPrime must be fast enough for n = 2147483647 (prime): stop at sqrt(n),
 *     and write the bound as i <= n / i or use long long i * i.
 *   - collatzSteps: 3n + 1 can exceed int range for large n, so keep the value in long long.
 *
 * Target complexity: digitSum/reverse O(log n); isPrime O(sqrt n);
 *   countPrimesUpTo O(n sqrt n) is fine here; collatzSteps O(steps).
 *
 * Run: make run FILE=curriculum/101_basics/03_loops.cpp
 *
 * (Stub note: delete the `(void)x;` lines once you use the parameter.)
 */
#include <climits>
#include <iostream>

#include "check.h"

// ===================== YOUR CODE =====================

int digitSum(int n) {
    // TODO. Hint: long long v = n; if (v < 0) v = -v; then repeat v % 10 and v /= 10.
    (void)n;
    return -1;
}

int reverseNumber(int n) {
    // TODO. Hint: peel off digits with % 10; before rev = rev * 10 + d, use long long.
    (void)n;
    return -1;
}

bool isPrime(int n) {
    // TODO. Hint: handle n < 2 first, then try divisors 2, 3, ... while d <= n / d.
    (void)n;
    return false;
}

int countPrimesUpTo(int n) {
    // TODO. Hint: loop 2..n and reuse isPrime.
    (void)n;
    return -1;
}

int collatzSteps(int n) {
    // TODO. Hint: keep the current value in a long long.
    (void)n;
    return -1;
}

// ===================== TESTS =====================

int main() {
    // digitSum
    CHECK_EQ(digitSum(0), 0);
    CHECK_EQ(digitSum(7), 7);
    CHECK_EQ(digitSum(123), 6);
    CHECK_EQ(digitSum(-4091), 14);
    CHECK_EQ(digitSum(1000000), 1);
    CHECK_EQ(digitSum(INT_MAX), 46);
    CHECK_EQ(digitSum(INT_MIN), 47);

    // reverseNumber
    CHECK_EQ(reverseNumber(0), 0);
    CHECK_EQ(reverseNumber(5), 5);
    CHECK_EQ(reverseNumber(123), 321);
    CHECK_EQ(reverseNumber(-123), -321);
    CHECK_EQ(reverseNumber(120), 21);
    CHECK_EQ(reverseNumber(-120), -21);
    CHECK_EQ(reverseNumber(1000), 1);
    CHECK_EQ(reverseNumber(1463847412), 2147483641);  // largest-ish that still fits
    CHECK_EQ(reverseNumber(1534236469), 0);           // overflows
    CHECK_EQ(reverseNumber(INT_MAX), 0);
    CHECK_EQ(reverseNumber(INT_MIN), 0);

    // isPrime
    CHECK_EQ(isPrime(INT_MIN), false);
    CHECK_EQ(isPrime(-7), false);
    CHECK_EQ(isPrime(0), false);
    CHECK_EQ(isPrime(1), false);
    CHECK_EQ(isPrime(2), true);
    CHECK_EQ(isPrime(3), true);
    CHECK_EQ(isPrime(4), false);
    CHECK_EQ(isPrime(9), false);
    CHECK_EQ(isPrime(25), false);
    CHECK_EQ(isPrime(49), false);
    CHECK_EQ(isPrime(97), true);
    CHECK_EQ(isPrime(7919), true);
    CHECK_EQ(isPrime(1000000007), true);
    CHECK_EQ(isPrime(999999937), true);
    CHECK_EQ(isPrime(1000000008), false);
    CHECK_EQ(isPrime(2147483647), true);
    CHECK_EQ(isPrime(2147483646), false);

    // countPrimesUpTo
    CHECK_EQ(countPrimesUpTo(-5), 0);
    CHECK_EQ(countPrimesUpTo(0), 0);
    CHECK_EQ(countPrimesUpTo(1), 0);
    CHECK_EQ(countPrimesUpTo(2), 1);
    CHECK_EQ(countPrimesUpTo(10), 4);
    CHECK_EQ(countPrimesUpTo(100), 25);
    CHECK_EQ(countPrimesUpTo(1000), 168);

    // collatzSteps
    CHECK_EQ(collatzSteps(1), 0);
    CHECK_EQ(collatzSteps(2), 1);
    CHECK_EQ(collatzSteps(6), 8);
    CHECK_EQ(collatzSteps(7), 16);
    CHECK_EQ(collatzSteps(27), 111);
    CHECK_EQ(collatzSteps(97), 118);
    CHECK_EQ(collatzSteps(1024), 10);

    return check::report();
}
