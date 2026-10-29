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
// Run: make run FILE=practice/cpp/binary_search/04_search_on_answer.cpp

#include "check.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <assert.h>
#include <unordered_map>

using ll = long long;

std::unordered_map<ll, int> daysCache;

ll min(ll a, ll b) {
    return (a<b)?a:b;
}

ll max(ll a, ll b) {
    return (a<b)?b:a;
}

ll findSmallestCapacity(const std::vector<int>& weights) {
    long long maxElem = INT_MIN;
    for (int elem : weights) {
        maxElem = max(maxElem, elem);
    }
    return maxElem;
}

ll findMaxCapacity(const std::vector<int>& weights) {
    ll capacity = 0;
    for (int elem : weights) {
        assert(elem >= 0);
        capacity += elem;
        assert(capacity >= 0); // no overflows, know if it happens
    }
    return capacity;
}

int checkNumberDays(const std::vector<int>& weights, const ll capacity) {
    if (daysCache.find(capacity) != daysCache.end()) {
        return daysCache[capacity];
    }
    int currentDays = 1;
    ll currentLoad = 0;
    for (int elem : weights) {
        if (elem > capacity) {
            // this will never pass through
            daysCache[capacity] = INT_MAX;
            return INT_MAX;
        }
        if (currentLoad + elem > capacity) {
            // new day
            currentDays++;
            currentLoad = 0;
        }
        currentLoad += elem;
    }
    daysCache[capacity] = currentDays;
    return currentDays;
}

long long minCapacityInternal(const std::vector<int>& weights, int days) {
    if (weights.empty()) return 0;
    ll smallestCapacity = findSmallestCapacity(weights); // most days
    ll largestCapacity = findMaxCapacity(weights); // least days

    // we know min and max, which is left and right.
    // we want to find a val where checkNumberDays == days
    int minDays = checkNumberDays(weights, largestCapacity);
    int maxDays = checkNumberDays(weights, smallestCapacity);
    assert(minDays == 1); // everything at once
    assert(maxDays >= 0 && (size_t) maxDays <= weights.size()); //biggest element size at once
    assert(minDays <= days);
    // assert(maxDays >= days);
    if (days > maxDays) {
        // our smallest capacity is okay
        return smallestCapacity;
    }
    /*
    At capacity min -> max.
        -> check the midpoint of the 'capacity', //more capacity => less days. We want to find appropriate days
            -> if midpointDays < days
    */
   ll leftCapacity = smallestCapacity;
   ll rightCapacity = largestCapacity;
   ll minCapacity = largestCapacity;
    while(leftCapacity <= rightCapacity) {
        if (leftCapacity == rightCapacity) {
            int numDays = checkNumberDays(weights, leftCapacity); 
            if (numDays <= days) {
                minCapacity = min(minCapacity, leftCapacity);
            }
            break;
        } else if (checkNumberDays(weights, leftCapacity) == days) {
            // lowest capacity in the whole array anyway
            minCapacity = min(minCapacity, leftCapacity);
            break;
        }
        ll size = rightCapacity - leftCapacity + 1;
        ll midCapacity = leftCapacity + (size/2);
        ll midNumDays = checkNumberDays(weights, midCapacity);
        if (midNumDays <= days) {
            minCapacity = min(minCapacity, midCapacity);
            // continue to check lower capacities
            rightCapacity = midCapacity-1;
            continue;
        } else if (midNumDays > days) {
            // increase capacity, days decrease means capcity increases
            leftCapacity = midCapacity + 1;
            continue;
        } else {
            assert(false); //never reach here

            // // midNumDays < days. This should be okay imo?
            // // decrease capacity is possible, meaning lower right range
            // rightCapacity = midCapacity - 1;
            // continue;
        }
    }
    daysCache.clear();
    return minCapacity;
}

long long minCapacity([[maybe_unused]] const std::vector<int>& weights, [[maybe_unused]] int days) {
    ll answer = minCapacityInternal(weights, days);
    daysCache.clear();
    return answer;
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
