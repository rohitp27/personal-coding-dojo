// Problem: Minimum Ship Capacity (binary search on the answer)
// Packages with the given weights must be shipped IN ORDER (no reordering) by a ship that
// makes one trip per day. Each day, load consecutive packages in order, never exceeding the
// ship's capacity C. Return the smallest C that ships all packages within `days` days.
//
// Examples:
//   weights = {1,2,3,4,5,6,7,8,9,10}, days = 5 -> 15
//       (days: 1+2+3+4+5, 6+7, 8, 9, 10)
//   weights = {3,2,2,4,1,4},          days = 3 -> 6
//   weights = {1,2,3,1,1},            days = 4 -> 3
//   weights = {7},                    days = 1 -> 7
//
// Constraints: 1 <= weights.size() <= 100,000; 1 <= weight <= 1,000,000,000; 1 <= days.
//              The answer can need 64 bits.
// Required complexity: O(n log(sum of weights)). Trying every capacity from the heaviest
// package upward is O(n * sum) and far too slow for the large tests.
//
// Think first:
//   1. What are the smallest and the largest values the answer could possibly take?
//      (These are your search bounds. What if days exceeds the number of packages?)
//   2. For a FIXED capacity, how do you decide in O(n) whether `days` days suffice?
//      Is that yes/no answer monotonic in the capacity, and why does that matter?
//   3. When feasible(mid) is true, should you set hi = mid or hi = mid - 1?
//
// Run: make run FILE=curriculum/102_fundamentals/binary_search/04_search_on_answer.cpp

#include "check.h"

#include <algorithm>
#include <numeric>
#include <vector>

//@REF
static bool feasible(const std::vector<int>& w, int days, long long cap) {
    int used = 1;
    long long cur = 0;
    for (int x : w) {
        if (cur + x > cap) {
            used++;
            cur = 0;
        }
        cur += x;
    }
    return used <= days;
}

long long minCapacity(const std::vector<int>& weights, int days) {
    long long lo = *std::max_element(weights.begin(), weights.end());
    long long hi = std::accumulate(weights.begin(), weights.end(), 0LL);
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (feasible(weights, days, mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
long long minCapacity([[maybe_unused]] const std::vector<int>& weights, [[maybe_unused]] int days) {
    // TODO
    return -1;
}

int main() {
    CHECK_EQ(minCapacity({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5), 15LL);
    CHECK_EQ(minCapacity({3, 2, 2, 4, 1, 4}, 3), 6LL);
    CHECK_EQ(minCapacity({1, 2, 3, 1, 1}, 4), 3LL);
    CHECK_EQ(minCapacity({7}, 1), 7LL);
    CHECK_EQ(minCapacity({5, 5, 5}, 3), 5LL);
    CHECK_EQ(minCapacity({5, 5, 5}, 1), 15LL);
    CHECK_EQ(minCapacity({5, 5, 5}, 10), 5LL);  // more days than packages
    CHECK_EQ(minCapacity({10, 50, 10, 50, 10}, 2), 70LL);
    CHECK_EQ(minCapacity({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1), 55LL);
    CHECK_EQ(minCapacity({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 10), 10LL);
    CHECK_EQ(minCapacity({9, 1, 1, 1, 9}, 3), 9LL);

    // Large inputs: scanning candidate capacities one by one is ~10^12+ operations.
    {
        std::vector<int> w(100000, 1000);
        CHECK_EQ(minCapacity(w, 10), 10000000LL);
        CHECK_EQ(minCapacity(w, 1), 100000000LL);
        CHECK_EQ(minCapacity(w, 100000), 1000LL);
        CHECK_EQ(minCapacity(w, 3), 33334000LL);  // ceil(100000 / 3) = 33334 packages per day
    }
    {
        std::vector<int> w(100000, 1000000000);
        CHECK_EQ(minCapacity(w, 1), 100000000000000LL);  // 10^14 needs 64 bits
        CHECK_EQ(minCapacity(w, 100000), 1000000000LL);
    }

    return check::report();
}
