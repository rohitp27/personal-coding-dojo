// Problem: Top K Frequent Elements
// Return k distinct values from nums, choosing higher frequencies first.
// When frequencies tie, prefer the smaller numeric value.
// Output order does not matter. Do not modify nums.
// k == 0 returns empty. Throw std::out_of_range if k < 0 or k exceeds
// the number of distinct values (including nonzero k for empty input).
// Use a frequency map and a priority_queue retaining at most k candidates.
// Target for k >= 2: expected O(n + u log k) time, O(u + k) extra space,
// where u is the number of distinct values. For k == 1, expected O(n + u).
// Example: {4, 4, 7, 7, 9}, k=1 -> {4}
// Run: make run FILE=practice/cpp/heaps/03_top_k_frequent.cpp

#include "check.h"
#include <algorithm>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>
#include <assert.h>

using Entry = std::pair<int, int>; // (frequency, value)

bool compare(const Entry& a, const Entry& b) {
    if (a.first == b.first) {
        return b.second > a.second;
    }
    return a.first > b.first;
}

std::vector<int> topKFrequent([[maybe_unused]] const std::vector<int>& nums,
                             [[maybe_unused]] int k) {
    // TODO: count frequencies, then select with your heap.
    std::unordered_map<int, int> mp;
    for(int elem : nums) {
        mp[elem]++;
    }
    if (k < 0 || (size_t)k > mp.size()) throw std::out_of_range("Bad k");
    if (k == 0) return {};

    std::priority_queue<Entry, std::vector<Entry>, decltype(&compare)> minHeap(&compare);
    for (auto it = mp.begin(); it != mp.end(); it++) {
        Entry current{it->second, it->first}; //(frequency, value)
        if (minHeap.size() == (size_t) k) { // remove before pushing
            if ( compare(current, minHeap.top()) ) { // means the top is a worse option
                minHeap.pop();
                minHeap.push(current);
            }
        } else {
            minHeap.push(current);
        }
    }
    std::vector<int> answer;
    while(!minHeap.empty()) {
        answer.push_back(minHeap.top().second);
        minHeap.pop();
    }
    return answer;
}

// ---- checks: keep these unchanged ----
static void checkTopK(const std::vector<int>& nums, int k,
                      std::vector<int> expected) {
    const auto original = nums;
    auto actual = topKFrequent(nums, k);
    std::sort(actual.begin(), actual.end());
    std::sort(expected.begin(), expected.end());
    CHECK_EQ(actual, expected);
    CHECK_EQ(nums, original);
}

static void checkInvalid(const std::vector<int>& nums, int k) {
    bool threw = false;
    try { (void)topKFrequent(nums, k); }
    catch (const std::out_of_range&) { threw = true; }
    CHECK(threw);
}

int main() {
    checkTopK({1, 1, 1, 2, 2, 3}, 2, {1, 2});
    checkTopK({4, 4, 7, 7, 9}, 1, {4});
    checkTopK({9, 9, 4, 4, 7, 7}, 2, {4, 7});
    checkTopK({9, 9, 9, 4, 4, 7, 7}, 2, {9, 4});
    checkTopK({5, 1, 3, 2}, 2, {1, 2});
    checkTopK({-2, -2, -1, -1, 0}, 1, {-2});
    checkTopK({0, 0, -1}, 1, {0});
    checkTopK({7}, 1, {7});
    checkTopK({3, 3, 3}, 1, {3});
    checkTopK({3, 1, 3, 2}, 3, {1, 2, 3});
    checkTopK({}, 0, {});
    checkTopK({1, 2}, 0, {});
    checkInvalid({}, 1);
    checkInvalid({1, 1}, 2);
    checkInvalid({1, 2}, -1);
    return check::report();
}
