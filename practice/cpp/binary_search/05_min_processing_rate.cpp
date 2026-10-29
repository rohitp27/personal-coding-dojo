// Problem: Minimum Processing Rate
// A worker has nonempty batches of items. At an integer rate r >= 1, each
// hour they process up to r items from ONE batch. If that batch finishes early,
// the unused part of the hour cannot be used for another batch.
// Return the smallest rate that finishes all batches within hours hours.
// Input contract: 1 <= batches.size() <= 100000; each batch is in [1, 10^9];
// batches.size() <= hours <= 10^9. Do not modify batches.
// Example: batches {3,6,7,11}, hours 8 -> rate 4.
// Run: make run FILE=practice/cpp/binary_search/05_min_processing_rate.cpp

#include "check.h"
#include <vector>

using ll = long long;

/*
Batches: [5, 8, 12]
Deadline: 7 hours
*/

int minProcessingRate(const std::vector<int>& batches, ll hours) {

    return -1;
}

int main() {
    CHECK_EQ(minProcessingRate({3, 6, 7, 11}, 8), 4);
    CHECK_EQ(minProcessingRate({30, 11, 23, 4, 20}, 5), 30);
    CHECK_EQ(minProcessingRate({30, 11, 23, 4, 20}, 6), 23);
    CHECK_EQ(minProcessingRate({1}, 1), 1);
    CHECK_EQ(minProcessingRate({9}, 2), 5);
    CHECK_EQ(minProcessingRate({8}, 2), 4);
    CHECK_EQ(minProcessingRate({2, 2, 2}, 5), 2);
    CHECK_EQ(minProcessingRate({2, 2, 2}, 6), 1);
    CHECK_EQ(minProcessingRate({1, 1, 1}, 1000000000LL), 1);
    CHECK_EQ(minProcessingRate({1000000000}, 3), 333333334);
    CHECK_EQ(minProcessingRate({1000000000, 1000000000, 1000000000},
                               1000000000LL), 4);
    const std::vector<int> batches{11, 3, 7, 6};
    CHECK_EQ(minProcessingRate(batches, 8), 4);
    CHECK_EQ(batches, (std::vector<int>{11, 3, 7, 6}));
    const std::vector<int> large(100000, 1000000000);
    CHECK_EQ(minProcessingRate(large, 100000), 1000000000);
    CHECK_EQ(minProcessingRate(large, 1000000000LL), 100000);
    return check::report();
}
