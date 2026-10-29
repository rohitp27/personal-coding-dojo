// Problem: Subarray Sum Equals K
// Count contiguous, nonempty subarrays whose sum equals k.
// Values may be positive, negative, or zero. Do not modify nums.
// Empty input returns 0. Use wide types for sums and the answer.
// Example: {1, 1, 1}, k=2 -> 2
// Example: {1, -1, 0}, k=0 -> 3
// Run: make run FILE=practice/cpp/hashing/06_subarray_sum_equals_k.cpp

#include "check.h"
#include <limits>
#include <unordered_map>
#include <vector>

long long countSubarrays([[maybe_unused]] const std::vector<int>& nums,
                         [[maybe_unused]] long long k) {
    // std::vector<long long> prefix(nums.size()+1, 0);
    std::unordered_map<long long, long long> prefixCount;
    long long sum = 0;
    long long count = 0;
    // prefix[0] = sum;
    prefixCount[0] = 1;

    for(size_t i = 0; i < nums.size(); i++) {
        // create the prefix arr
        int curr = nums[i];
        sum += curr;
        // prefix[i+1] = sum;

        long long prefixSum = sum;
        long long complement = prefixSum - k;
        if (prefixCount.find(complement) != prefixCount.end()) {
            // exists
            count += prefixCount[complement];
        }
        prefixCount[prefixSum] += 1;
    }

    // std::unordered_map<long long, long long> prefixCount;
    // for(size_t i = 0; i < prefix.size(); i++) {
    //     long long prefixSum = prefix[i];
    //     long long complement = prefixSum - k;
    //     if (prefixCount.find(complement) != prefixCount.end()) {
    //         // complement exists
    //         count += prefixCount[complement];
    //     } 
    //     // register this prefix sum
    //     prefixCount[prefixSum] += 1;
    // }
    return count;
}

// ---- checks: keep these unchanged ----
int main() {
    CHECK_EQ(countSubarrays({1, 1, 1}, 2), 2LL);
    CHECK_EQ(countSubarrays({1, 2, 3}, 3), 2LL);
    CHECK_EQ(countSubarrays({1, -1, 0}, 0), 3LL);
    CHECK_EQ(countSubarrays({}, 0), 0LL);
    CHECK_EQ(countSubarrays({4, -1}, 3), 1LL);
    CHECK_EQ(countSubarrays({3}, 3), 1LL);
    CHECK_EQ(countSubarrays({3}, 0), 0LL);
    CHECK_EQ(countSubarrays({0}, 0), 1LL);
    CHECK_EQ(countSubarrays({0, 0, 0}, 0), 6LL);
    CHECK_EQ(countSubarrays({1, 2, 3}, 99), 0LL);
    CHECK_EQ(countSubarrays({-1, -1, 1}, -1), 3LL);
    CHECK_EQ(countSubarrays({1, -1, 1, -1}, 0), 4LL);
    const int hi = std::numeric_limits<int>::max();
    CHECK_EQ(countSubarrays({hi, hi}, 2LL * hi), 1LL);
    CHECK_EQ(countSubarrays({hi, hi, -hi}, hi), 3LL);
    const std::vector<int> input{2, -1, 3};
    CHECK_EQ(countSubarrays(input, 3), 1LL);
    CHECK_EQ(input, (std::vector<int>{2, -1, 3}));
    CHECK_EQ(countSubarrays(std::vector<int>(100000, 0), 0), 5000050000LL);
    return check::report();
}
