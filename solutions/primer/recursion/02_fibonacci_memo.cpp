// Problem: Fibonacci three ways, with fib(0) = 0, fib(1) = 1.
//   fib(n)             recursive, with a memo table (std::unordered_map<int, long long>)
//                      so fib(50) returns instantly
//   fibBottomUp(n)     iterative, filling a std::vector<long long> of size n + 1
//   climbStairs(n)     number of ways to climb n steps taking 1 or 2 at a time
//                      (climbStairs(0) = 1; hint: same shape as Fibonacci)
//
// Examples:
//   fib(10)            -> 55
//   fibBottomUp(10)    -> 55
//   climbStairs(4)     -> 5     (1111, 112, 121, 211, 22)
//
// Edge cases: n = 0, n = 1, large n (50) must not time out or overflow long long.
//
// Run with: make run FILE=practice/cpp/recursion/02_fibonacci_memo.cpp
//
// Syntax reminder:
//   long long fibMemo(int n, std::unordered_map<int, long long>& memo);   // pass memo by reference
//   auto it = memo.find(n);  if (it != memo.end()) return it->second;
//   memo[n] = result;
//   std::vector<long long> dp(n + 1, 0);   // n + 1 zeros
//   dp[i] = dp[i - 1] + dp[i - 2];

#include <unordered_map>
#include <vector>

#include "check.h"

static long long fibMemo(int n, std::unordered_map<int, long long>& memo) {
    if (n <= 1) return n;
    auto it = memo.find(n);
    if (it != memo.end()) return it->second;
    long long result = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);
    memo[n] = result;
    return result;
}

long long fib(int n) {
    std::unordered_map<int, long long> memo;
    return fibMemo(n, memo);
}

long long fibBottomUp(int n) {
    if (n <= 1) return n;
    std::vector<long long> dp(n + 1, 0);
    dp[1] = 1;
    for (int i = 2; i <= n; i++) dp[i] = dp[i - 1] + dp[i - 2];
    return dp[n];
}

long long climbStairs(int n) {
    return fib(n + 1);
}

int main() {
    CHECK_EQ(fib(0), 0LL);
    CHECK_EQ(fib(1), 1LL);
    CHECK_EQ(fib(10), 55LL);
    CHECK_EQ(fib(50), 12586269025LL);

    CHECK_EQ(fibBottomUp(0), 0LL);
    CHECK_EQ(fibBottomUp(1), 1LL);
    CHECK_EQ(fibBottomUp(10), 55LL);
    CHECK_EQ(fibBottomUp(50), 12586269025LL);

    CHECK_EQ(climbStairs(0), 1LL);
    CHECK_EQ(climbStairs(1), 1LL);
    CHECK_EQ(climbStairs(2), 2LL);
    CHECK_EQ(climbStairs(4), 5LL);
    CHECK_EQ(climbStairs(10), 89LL);

    return check::report();
}
