// Problem: Enumerate subsets with recursion (include / exclude each element).
//   subsets(nums)                     all subsets of nums (distinct elements),
//                                     each subset keeping the input order; any order of subsets
//   countSubsetsWithSum(nums, target) how many subsets (including the empty one)
//                                     add up to target
//
// Examples:
//   subsets({1,2})                         -> {}, {1}, {2}, {1,2}   (in any order)
//   countSubsetsWithSum({1,2,3}, 3)        -> 2     ({3} and {1,2})
//
// Edge cases: empty input has exactly one subset, the empty one;
//             countSubsetsWithSum({}, 0) is 1.
//
// Run with: make run FILE=practice/cpp/recursion/03_subsets.cpp
//
// Syntax reminder:
//   void build(const std::vector<int>& nums, size_t i,
//              std::vector<int>& current, std::vector<std::vector<int>>& out);
//   current.push_back(nums[i]);  build(...);  current.pop_back();   // choose, recurse, un-choose
//   out.push_back(current);                                          // copies current
//   std::sort(out.begin(), out.end());                               // vectors compare lexicographically

#include <algorithm>
#include <vector>

#include "check.h"

static void build(const std::vector<int>& nums, size_t i, std::vector<int>& current,
                  std::vector<std::vector<int>>& out) {
    if (i == nums.size()) {
        out.push_back(current);
        return;
    }
    build(nums, i + 1, current, out);  // exclude
    current.push_back(nums[i]);        // include
    build(nums, i + 1, current, out);
    current.pop_back();
}

std::vector<std::vector<int>> subsets(const std::vector<int>& nums) {
    std::vector<std::vector<int>> out;
    std::vector<int> current;
    build(nums, 0, current, out);
    return out;
}

static int countFrom(const std::vector<int>& nums, size_t i, int remaining) {
    if (i == nums.size()) return remaining == 0 ? 1 : 0;
    return countFrom(nums, i + 1, remaining) + countFrom(nums, i + 1, remaining - nums[i]);
}

int countSubsetsWithSum(const std::vector<int>& nums, int target) {
    return countFrom(nums, 0, target);
}

// Sort the outer vector so the tests do not depend on generation order.
static std::vector<std::vector<int>> sortedSubsets(const std::vector<int>& nums) {
    auto r = subsets(nums);
    std::sort(r.begin(), r.end());
    return r;
}

int main() {
    CHECK_EQ(sortedSubsets({}), (std::vector<std::vector<int>>{{}}));
    CHECK_EQ(sortedSubsets({7}), (std::vector<std::vector<int>>{{}, {7}}));
    CHECK_EQ(sortedSubsets({1, 2}), (std::vector<std::vector<int>>{{}, {1}, {1, 2}, {2}}));
    CHECK_EQ(sortedSubsets({1, 2, 3}),
             (std::vector<std::vector<int>>{{}, {1}, {1, 2}, {1, 2, 3}, {1, 3}, {2}, {2, 3}, {3}}));
    CHECK_EQ(subsets({1, 2, 3, 4}).size(), size_t{16});

    CHECK_EQ(countSubsetsWithSum({}, 0), 1);
    CHECK_EQ(countSubsetsWithSum({}, 5), 0);
    CHECK_EQ(countSubsetsWithSum({1, 2, 3}, 3), 2);
    CHECK_EQ(countSubsetsWithSum({1, 1, 1}, 2), 3);
    CHECK_EQ(countSubsetsWithSum({5, 6}, 4), 0);

    return check::report();
}
