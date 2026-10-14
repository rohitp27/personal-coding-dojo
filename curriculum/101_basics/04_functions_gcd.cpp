/*
 * Problem: Functions - gcd and friends
 *   Write small, single-purpose functions and build bigger ones from them.
 *
 *     gcd(a, b)               Greatest common divisor, a, b >= 0. gcd(0, 0) = 0.
 *     lcm(a, b)               Least common multiple, a, b >= 0. lcm with 0 is 0.
 *                             Divide before multiplying: a / gcd * b.
 *     reduceFraction(n, d)    Reduce n/d (d != 0) to lowest terms and return
 *                             {numerator, denominator} with denominator > 0.
 *                             A zero numerator reduces to {0, 1}.
 *     gcdOfVector(v)          gcd of all elements (use absolute values);
 *                             0 for an empty vector.
 *     powMod(base, exp, mod)  (base ^ exp) % mod via fast exponentiation (square and
 *                             multiply), 0 <= base, 0 <= exp, 1 <= mod <= 1e9.
 *
 * Examples:
 *   gcd(48, 18)             -> 6
 *   lcm(4, 6)               -> 12
 *   reduceFraction(6, -8)   -> {-3, 4}
 *   gcdOfVector({-4, 6, 10})-> 2
 *   powMod(2, 10, 1000)     -> 24
 *
 * Constraints / edge cases:
 *   - gcd(0, x) = x. Use the Euclidean algorithm (a, b) -> (b, a % b).
 *   - lcm of two ~2^31 numbers needs long long.
 *   - powMod with exp = 0 gives 1 % mod (so 0 when mod == 1).
 *   - Intermediate products in powMod can reach (1e9)^2, so use long long.
 *
 * Target complexity: gcd O(log min(a,b)); powMod O(log exp).
 *
 * Run: make run FILE=curriculum/101_basics/04_functions_gcd.cpp
 *
 * (Stub note: delete the `(void)x;` lines once you use the parameter.)
 */
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

#include "check.h"

// ===================== YOUR CODE =====================

long long gcd(long long a, long long b) {
    // TODO. Hint: while (b != 0) { long long t = a % b; a = b; b = t; }
    (void)a;
    (void)b;
    return -1;
}

long long lcm(long long a, long long b) {
    // TODO. Hint: return 0 if either is 0, else a / gcd(a, b) * b.
    (void)a;
    (void)b;
    return -1;
}

std::pair<int, int> reduceFraction(int num, int den) {
    // TODO. Hint: g = gcd(|num|, |den|); then fix the sign so only the numerator is negative.
    (void)num;
    (void)den;
    return {0, 0};
}

int gcdOfVector(const std::vector<int>& v) {
    // TODO. Hint: fold gcd over the elements, starting from 0.
    (void)v;
    return -1;
}

long long powMod(long long base, long long exp, long long mod) {
    // TODO. Hint: result = 1 % mod; loop: if (exp & 1) result = result * base % mod; base = base * base % mod; exp >>= 1.
    (void)base;
    (void)exp;
    (void)mod;
    return -1;
}

// ===================== TESTS =====================

int main() {
    // gcd
    CHECK_EQ(gcd(48, 18), 6LL);
    CHECK_EQ(gcd(18, 48), 6LL);
    CHECK_EQ(gcd(17, 5), 1LL);
    CHECK_EQ(gcd(0, 5), 5LL);
    CHECK_EQ(gcd(5, 0), 5LL);
    CHECK_EQ(gcd(0, 0), 0LL);
    CHECK_EQ(gcd(7, 7), 7LL);
    CHECK_EQ(gcd(1, 1000000007), 1LL);
    CHECK_EQ(gcd(1000000000000000000LL, 250000000000LL), 250000000000LL);

    // lcm
    CHECK_EQ(lcm(4, 6), 12LL);
    CHECK_EQ(lcm(6, 4), 12LL);
    CHECK_EQ(lcm(0, 5), 0LL);
    CHECK_EQ(lcm(5, 0), 0LL);
    CHECK_EQ(lcm(7, 7), 7LL);
    CHECK_EQ(lcm(1, 9), 9LL);
    CHECK_EQ(lcm(2147483647LL, 2147483646LL), 4611686011984936962LL);

    // reduceFraction
    CHECK_EQ(reduceFraction(6, 8), std::make_pair(3, 4));
    CHECK_EQ(reduceFraction(6, -8), std::make_pair(-3, 4));
    CHECK_EQ(reduceFraction(-6, 8), std::make_pair(-3, 4));
    CHECK_EQ(reduceFraction(-4, -2), std::make_pair(2, 1));
    CHECK_EQ(reduceFraction(0, -5), std::make_pair(0, 1));
    CHECK_EQ(reduceFraction(0, 7), std::make_pair(0, 1));
    CHECK_EQ(reduceFraction(7, 1), std::make_pair(7, 1));
    CHECK_EQ(reduceFraction(5, 7), std::make_pair(5, 7));
    CHECK_EQ(reduceFraction(1000000000, 500000000), std::make_pair(2, 1));

    // gcdOfVector
    CHECK_EQ(gcdOfVector({}), 0);
    CHECK_EQ(gcdOfVector({7}), 7);
    CHECK_EQ(gcdOfVector({-7}), 7);
    CHECK_EQ(gcdOfVector({0, 0}), 0);
    CHECK_EQ(gcdOfVector({0, 9}), 9);
    CHECK_EQ(gcdOfVector({-4, 6, 10}), 2);
    CHECK_EQ(gcdOfVector({12, 18, 24}), 6);
    CHECK_EQ(gcdOfVector({3, 5, 7}), 1);

    // powMod
    CHECK_EQ(powMod(2, 10, 1000), 24LL);
    CHECK_EQ(powMod(2, 0, 1000), 1LL);
    CHECK_EQ(powMod(5, 0, 1), 0LL);
    CHECK_EQ(powMod(0, 0, 7), 1LL);
    CHECK_EQ(powMod(0, 5, 7), 0LL);
    CHECK_EQ(powMod(7, 13, 100), 7LL);
    CHECK_EQ(powMod(3, 200, 1000000007), 136318165LL);
    CHECK_EQ(powMod(2, 62, 1000000007), 145586002LL);
    CHECK_EQ(powMod(123456789, 987654321, 1000000007), 652541198LL);
    CHECK_EQ(powMod(1000000000, 1000000000, 999999999), 1LL);

    return check::report();
}
