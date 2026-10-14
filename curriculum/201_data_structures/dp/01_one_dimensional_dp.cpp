// Problem: Three classic 1D dynamic programs.
//   long long robMax(const std::vector<int>& nums)
//        Pick values from nums so that no two picked values are adjacent; return the maximum sum.
//        nums[i] >= 0. Empty input -> 0.
//   int minCoins(const std::vector<int>& coins, int amount)
//        Fewest coins (unlimited supply of each denomination) that sum to exactly amount, or -1 if impossible.
//        amount == 0 -> 0. Greedy is NOT always correct here.
//   long long countWays(const std::vector<int>& coins, int amount)
//        Number of COMBINATIONS (order does not matter) of coins that sum to amount. amount == 0 -> 1.
//
// Hint: define dp[i] in one sentence ("best answer using the first i houses" / "fewest coins for amount i")
//   before writing a loop. For countWays, the loop order (coins outside, amounts inside) is what makes
//   {1,2} and {2,1} count once.
//
// Examples:
//   robMax({2,7,9,3,1}) = 12 (2 + 9 + 1)        minCoins({1,3,4}, 6) = 2 (3 + 3)
//   minCoins({2}, 3) = -1                       countWays({1,2,5}, 5) = 4
//
// Constraints: nums up to 1,000,000 entries (sum fits in long long); amount up to 1,000,000; up to 20 coin
//   values, each >= 1.
// Required complexity: robMax O(n) time, O(1) extra space; coin problems O(amount * coins) time, O(amount) space.
// Run: make run FILE=curriculum/201_data_structures/dp/01_one_dimensional_dp.cpp

#include "check.h"

#include <algorithm>
#include <climits>
#include <vector>

// ===== YOUR CODE BELOW (stub) =====
long long robMax([[maybe_unused]] const std::vector<int>& nums) {
    // TODO
    return 0;
}

int minCoins([[maybe_unused]] const std::vector<int>& coins, [[maybe_unused]] int amount) {
    // TODO
    return 0;
}

long long countWays([[maybe_unused]] const std::vector<int>& coins, [[maybe_unused]] int amount) {
    // TODO
    return 0;
}

// ===== TESTS (do not edit below) =====
using V = std::vector<int>;

int main() {
    // robMax
    CHECK_EQ(robMax({}), 0LL);
    CHECK_EQ(robMax({5}), 5LL);
    CHECK_EQ(robMax({1, 2}), 2LL);
    CHECK_EQ(robMax({2, 7, 9, 3, 1}), 12LL);
    CHECK_EQ(robMax({1, 2, 3, 1}), 4LL);
    CHECK_EQ(robMax({2, 1, 1, 2}), 4LL);  // skipping two in a row is allowed
    CHECK_EQ(robMax({0, 0, 0}), 0LL);
    CHECK_EQ(robMax({10, 1, 1, 10}), 20LL);
    CHECK_EQ(robMax({1, 100, 1, 1, 100, 1}), 200LL);

    // minCoins
    CHECK_EQ(minCoins({1, 2, 5}, 11), 3);
    CHECK_EQ(minCoins({2}, 3), -1);
    CHECK_EQ(minCoins({1}, 0), 0);
    CHECK_EQ(minCoins({}, 0), 0);
    CHECK_EQ(minCoins({}, 5), -1);
    CHECK_EQ(minCoins({1, 3, 4}, 6), 2);  // greedy would take 4 + 1 + 1
    CHECK_EQ(minCoins({5, 3}, 7), -1);
    CHECK_EQ(minCoins({7}, 7), 1);
    CHECK_EQ(minCoins({186, 419, 83, 408}, 6249), 20);
    CHECK_EQ(minCoins({2, 5, 10, 1}, 27), 4);  // 10 + 10 + 5 + 2

    // countWays
    CHECK_EQ(countWays({1, 2, 5}, 5), 4LL);
    CHECK_EQ(countWays({2}, 3), 0LL);
    CHECK_EQ(countWays({10}, 10), 1LL);
    CHECK_EQ(countWays({1, 2, 5}, 0), 1LL);
    CHECK_EQ(countWays({}, 0), 1LL);
    CHECK_EQ(countWays({}, 3), 0LL);
    CHECK_EQ(countWays({1, 2, 3}, 4), 4LL);  // 1111, 112, 22, 13
    CHECK_EQ(countWays({1, 5, 10, 25, 50}, 100), 292LL);
    CHECK_EQ(countWays({1, 5, 10, 25, 50, 100}, 100), 293LL);

    // large inputs
    {
        const int n = 1000000;
        CHECK_EQ(robMax(V(n, 1)), 500000LL);
        V ramp(n);
        for (int i = 0; i < n; i++) ramp[i] = i;
        // best is every odd index 1,3,...,n-1: 500000 terms, sum = 500000^2
        CHECK_EQ(robMax(ramp), 250000000000LL);
        V big(n, INT_MAX);
        CHECK_EQ(robMax(big), 500000LL * INT_MAX);
    }
    CHECK_EQ(minCoins({1, 5, 10, 25, 100}, 99999), 1008);  // 999 dollars + 75c + 20c + 4c = 999 + 3 + 2 + 4
    CHECK_EQ(minCoins({2}, 100000), 50000);
    CHECK_EQ(minCoins({2}, 99999), -1);
    CHECK_EQ(minCoins({1}, 1000000), 1000000);
    CHECK_EQ(countWays({1}, 1000000), 1LL);
    CHECK_EQ(countWays({1, 2}, 1000000), 500001LL);
    CHECK_EQ(countWays({7, 11}, 1000000), countWays({11, 7}, 1000000));  // order of denominations is irrelevant
    return check::report();
}
