// Problem: Climbing Stairs (memoized recursion)
// You climb a staircase of n steps, taking 1 or 2 steps at a time. Return the number of
// distinct sequences of moves that reach exactly step n. Climbing 0 steps has exactly one
// way (do nothing).
//
// Examples:
//   n = 0  -> 1
//   n = 1  -> 1
//   n = 2  -> 2    (1+1, 2)
//   n = 3  -> 3    (1+1+1, 1+2, 2+1)
//   n = 5  -> 8
//
// Constraints: 0 <= n <= 80 (the answer fits in a signed 64-bit integer).
// Required complexity: O(n) time and space. Write it as a TOP-DOWN recursion with a memo
// table; the plain recursion without a memo makes an exponential number of calls and will
// not finish for n = 80.
//
// Hint: the last move was either a 1-step from n-1 or a 2-step from n-2, so
// ways(n) = ways(n-1) + ways(n-2) with base cases for n = 0 and n = 1. Store each
// ways(i) the first time you compute it (e.g. in a vector<long long> initialised to -1 and
// passed by reference) and return the stored value on every later call.
//
// Run: make run FILE=curriculum/102_fundamentals/recursion_and_sorting/01_memo_climb_stairs.cpp

#include "check.h"

#include <vector>

long long climbStairs([[maybe_unused]] int n) {
    // TODO
    return -1;
}

int main() {
    CHECK_EQ(climbStairs(0), 1LL);
    CHECK_EQ(climbStairs(1), 1LL);
    CHECK_EQ(climbStairs(2), 2LL);
    CHECK_EQ(climbStairs(3), 3LL);
    CHECK_EQ(climbStairs(4), 5LL);
    CHECK_EQ(climbStairs(5), 8LL);
    CHECK_EQ(climbStairs(10), 89LL);
    CHECK_EQ(climbStairs(20), 10946LL);
    CHECK_EQ(climbStairs(45), 1836311903LL);
    CHECK_EQ(climbStairs(50), 20365011074LL);
    // naive recursion makes more than 10^16 calls here
    CHECK_EQ(climbStairs(80), 37889062373143906LL);

    return check::report();
}
