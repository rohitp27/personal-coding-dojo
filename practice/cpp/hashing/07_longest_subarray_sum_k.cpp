// Problem: Longest Subarray Sum Equals K
// Return the length of the longest contiguous, nonempty subarray summing to k.
// Return 0 if none exists. Values may be positive, negative, or zero.
// Do not modify nums. Use wide arithmetic for sums.
// Example: {1, -1, 5, -2, 3}, k=3 -> 4
// Run: make run FILE=practice/cpp/hashing/07_longest_subarray_sum_k.cpp

#include "check.h"
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <vector>
#include <assert.h>

int min(int a, int b) {
    return (a<b)?a:b;
}

int max(int a, int b) {
    return (a<b)?b:a;
}

int longestSubarray([[maybe_unused]] const std::vector<int>& nums,
                    [[maybe_unused]] long long k) {
    std::unordered_map<long long, long long> mp;
    long long prefixSum = 0;
    mp[prefixSum] = -1;
    int longest = 0;
    for(size_t i = 0; i < nums.size(); i++) {
        int current = nums[i];
        prefixSum += current;
        long long complement = prefixSum - k;
        if (mp.find(complement) != mp.end()) {
            long long j = mp[complement];
            // assert(i > j);
            longest = max(longest, i-j);
        }
        if (mp.find(prefixSum) != mp.end()) {
            //alerad exists
            // don't add
        } else {
            mp[prefixSum] = i;
        }
    }
    return longest;
}

// ---- checks: keep these unchanged ----
int main() {
    CHECK_EQ(longestSubarray({1, -1, 5, -2, 3}, 3), 4);
    CHECK_EQ(longestSubarray({-2, -1, 2, 1}, 1), 2);
    CHECK_EQ(longestSubarray({}, 0), 0);
    CHECK_EQ(longestSubarray({3}, 3), 1);
    CHECK_EQ(longestSubarray({3}, 0), 0);
    CHECK_EQ(longestSubarray({0, 0, 0}, 0), 3);
    CHECK_EQ(longestSubarray({1, -1, 1, -1}, 0), 4);
    CHECK_EQ(longestSubarray({9, 1, -1, 2, -2}, 0), 4);
    CHECK_EQ(longestSubarray({1, 2, 3}, 99), 0);
    CHECK_EQ(longestSubarray({4, -1}, 3), 2);
    CHECK_EQ(longestSubarray({2, -2, 2, -2, 3}, 3), 5);
    CHECK_EQ(longestSubarray({1, 2, 1, 2}, 3), 2);
    const int hi = std::numeric_limits<int>::max();
    CHECK_EQ(longestSubarray({hi, hi}, 2LL * hi), 2);
    CHECK_EQ(longestSubarray({hi, hi, -hi}, hi), 3);
    const std::vector<int> input{7, 2, -1, 2};
    CHECK_EQ(longestSubarray(input, 3), 3);
    CHECK_EQ(input, (std::vector<int>{7, 2, -1, 2}));
    CHECK_EQ(longestSubarray(std::vector<int>(100000, 0), 0), 100000);
    return check::report();
}
