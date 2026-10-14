/*
 * Problem: Integer arithmetic pitfalls
 *   Integer division truncates toward zero, % can be negative, and signed overflow is
 *   undefined behavior. Implement these helpers without ever overflowing `int`
 *   (hint: widen to long long for intermediate results, or reorder the math).
 *
 *     midpoint(lo, hi)     Average of lo and hi (lo <= hi), rounded toward negative
 *                          infinity (floor). Must not overflow for any ints.
 *     ceilDiv(a, b)        ceil(a / b) for a >= 0, b > 0. Must not overflow
 *                          (the classic (a + b - 1) / b does).
 *     addOverflows(a, b)   True iff a + b does not fit in int. Do not actually
 *                          perform an overflowing int addition.
 *     floorMod(a, m)       Mathematical modulo for m > 0: result always in [0, m).
 *                          C++'s a % m is negative when a is negative.
 *     factorial(n)         n! as long long for 0 <= n <= 20; return -1 for n < 0 or
 *                          when n! would not fit in long long (n >= 21).
 *
 * Examples:
 *   midpoint(-3, 0) -> -2        (true average -1.5, floored)
 *   ceilDiv(7, 2)   -> 4
 *   floorMod(-1, 5) -> 4
 *   factorial(5)    -> 120
 *
 * Constraints / edge cases:
 *   - INT_MIN and INT_MAX as inputs; hi - lo itself can exceed int range.
 *   - Negative numerators for floorMod; exact multiples give 0.
 *   - factorial(0) == 1; factorial(20) fits, factorial(21) does not.
 *
 * Target complexity: O(1) for the first four, O(n) for factorial.
 *
 * Run: make run FILE=curriculum/101_basics/02_integer_pitfalls.cpp
 *
 * (Stub note: delete the `(void)x;` lines once you use the parameter.)
 */
#include <climits>
#include <iostream>

#include "check.h"

// ===================== YOUR CODE =====================

int midpoint(int lo, int hi) {
    // TODO. Hint: lo + (hi - lo) / 2, computed in long long. Why does floor work here?
    (void)lo;
    (void)hi;
    return 0;
}

int ceilDiv(int a, int b) {
    // TODO. Hint: a / b, plus 1 if there is a remainder.
    (void)a;
    (void)b;
    return 0;
}

bool addOverflows(int a, int b) {
    // TODO. Hint: widen to long long and compare against INT_MIN / INT_MAX.
    (void)a;
    (void)b;
    return false;
}

int floorMod(int a, int m) {
    // TODO. Hint: take a % m, and if it is negative add m.
    (void)a;
    (void)m;
    return -1;
}

long long factorial(int n) {
    // TODO. Hint: before multiplying, check result > LLONG_MAX / i. Or just cap at 20.
    (void)n;
    return 0;
}

// ===================== TESTS =====================

int main() {
    // midpoint
    CHECK_EQ(midpoint(0, 10), 5);
    CHECK_EQ(midpoint(3, 4), 3);
    CHECK_EQ(midpoint(-3, 0), -2);
    CHECK_EQ(midpoint(-4, -1), -3);
    CHECK_EQ(midpoint(7, 7), 7);
    CHECK_EQ(midpoint(INT_MAX - 1, INT_MAX), INT_MAX - 1);
    CHECK_EQ(midpoint(INT_MAX, INT_MAX), INT_MAX);
    CHECK_EQ(midpoint(INT_MIN, INT_MAX), -1);
    CHECK_EQ(midpoint(INT_MIN, INT_MIN), INT_MIN);
    CHECK_EQ(midpoint(INT_MIN, 0), INT_MIN / 2);
    CHECK_EQ(midpoint(1000000000, 2000000000), 1500000000);

    // ceilDiv
    CHECK_EQ(ceilDiv(7, 2), 4);
    CHECK_EQ(ceilDiv(10, 5), 2);
    CHECK_EQ(ceilDiv(11, 5), 3);
    CHECK_EQ(ceilDiv(0, 5), 0);
    CHECK_EQ(ceilDiv(1, 1000), 1);
    CHECK_EQ(ceilDiv(INT_MAX, 2), 1073741824);
    CHECK_EQ(ceilDiv(INT_MAX, INT_MAX), 1);
    CHECK_EQ(ceilDiv(INT_MAX - 1, INT_MAX), 1);
    CHECK_EQ(ceilDiv(INT_MAX, 1), INT_MAX);

    // addOverflows
    CHECK_EQ(addOverflows(1, 2), false);
    CHECK_EQ(addOverflows(INT_MAX, 1), true);
    CHECK_EQ(addOverflows(INT_MAX, 0), false);
    CHECK_EQ(addOverflows(INT_MAX, INT_MIN), false);
    CHECK_EQ(addOverflows(INT_MIN, -1), true);
    CHECK_EQ(addOverflows(INT_MIN, 0), false);
    CHECK_EQ(addOverflows(INT_MIN, INT_MIN), true);
    CHECK_EQ(addOverflows(INT_MAX, INT_MAX), true);
    CHECK_EQ(addOverflows(1000000000, 1147483647), false);
    CHECK_EQ(addOverflows(1000000001, 1147483647), true);
    CHECK_EQ(addOverflows(1073741824, 1073741824), true);
    CHECK_EQ(addOverflows(-5, 5), false);

    // floorMod
    CHECK_EQ(floorMod(7, 5), 2);
    CHECK_EQ(floorMod(-1, 5), 4);
    CHECK_EQ(floorMod(-10, 5), 0);
    CHECK_EQ(floorMod(0, 5), 0);
    CHECK_EQ(floorMod(-7, 3), 2);
    CHECK_EQ(floorMod(5, 1), 0);
    CHECK_EQ(floorMod(-5, 1), 0);
    CHECK_EQ(floorMod(INT_MIN, 7), 5);
    CHECK_EQ(floorMod(INT_MAX, 7), 1);
    CHECK_EQ(floorMod(INT_MIN, INT_MAX), INT_MAX - 1);

    // factorial
    CHECK_EQ(factorial(0), 1LL);
    CHECK_EQ(factorial(1), 1LL);
    CHECK_EQ(factorial(5), 120LL);
    CHECK_EQ(factorial(12), 479001600LL);
    CHECK_EQ(factorial(13), 6227020800LL);  // already past int range
    CHECK_EQ(factorial(20), 2432902008176640000LL);
    CHECK_EQ(factorial(21), -1LL);
    CHECK_EQ(factorial(100), -1LL);
    CHECK_EQ(factorial(-1), -1LL);

    return check::report();
}
