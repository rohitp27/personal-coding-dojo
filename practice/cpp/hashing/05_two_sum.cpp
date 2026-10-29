// Problem: Two Sum
// Return indices {i, j}, i < j, of two distinct elements summing to target.
// Return {} if no pair exists. If several pairs exist, any valid pair is accepted.
// Duplicates are allowed. Do not modify nums.
// Inputs and target are int; avoid overflow when computing a complement.
// Target: O(n) expected time, O(n) extra space.
// Run: make run FILE=practice/cpp/hashing/05_two_sum.cpp

#include "check.h"
#include <limits>
#include <unordered_map>
#include <vector>

std::vector<int> twoSum([[maybe_unused]] const std::vector<int>& nums,
                        [[maybe_unused]] int target) {
    std::unordered_map<long, size_t> mp;
    for(size_t i = 0; i < nums.size(); i++) {
        int num = nums[i];
        long complement = (long)target-num;
        if (mp.find(complement) != mp.end()) {
            return { (int) mp[complement], (int)i};
        } else {
            mp[num] = i;
        }
    } 
    return {};
}

// ---- checks: keep these unchanged ----
static void checkPair(const std::vector<int>& nums, int target, bool exists) {
    const auto original = nums;
    const auto answer = twoSum(nums, target);
    CHECK_EQ(nums, original);
    if (!exists) {
        CHECK(answer.empty());
        return;
    }
    CHECK_EQ(answer.size(), size_t{2});
    if (answer.size() != 2) return;
    const int i = answer[0], j = answer[1];
    const bool valid = i >= 0 && i < j && static_cast<size_t>(j) < nums.size();
    CHECK(valid);
    if (valid) CHECK_EQ(static_cast<long long>(nums[i]) + nums[j],
                        static_cast<long long>(target));
}

int main() {
    checkPair({2, 7, 11, 15}, 9, true);
    checkPair({3, 3}, 6, true);
    checkPair({3}, 6, false);
    checkPair({}, 0, false);
    checkPair({0, 0}, 0, true);
    checkPair({-3, 4, 3, 90}, 0, true);
    checkPair({1, 2, 3}, 10, false);
    checkPair({1, 4, 2, 3}, 5, true);
    checkPair({3, 2, 4}, 6, true);
    const int lo = std::numeric_limits<int>::min();
    const int hi = std::numeric_limits<int>::max();
    checkPair({lo, hi}, -1, true);
    checkPair({lo, -1}, hi, false);
    checkPair({hi, 1}, lo, false);
    return check::report();
}
