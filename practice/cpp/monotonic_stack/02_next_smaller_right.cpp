// Problem: Next Smaller Index to the Right
// For each position, return the INDEX of the first strictly smaller element
// to its right. Return nums.size() when none exists (the boundary after the array).
// Equal values do not count as smaller. Do not modify nums.
// Example: {2, 1, 5, 6, 2, 3} -> {1, 6, 4, 4, 6, 6}
// Target: O(n) time, O(n) extra space.
// Before coding: describe which indices are waiting and their value order.
// Run: make run FILE=practice/cpp/monotonic_stack/02_next_smaller_right.cpp

#include "check.h"
#include <stack>
#include <vector>

std::vector<int> nextSmallerRight(const std::vector<int>& nums) {
    std::vector<int> answer(nums.size(), nums.size());
    std::stack<int> stk;
    for (size_t i = 0; i < nums.size(); i++) {
        while(!stk.empty() && nums[stk.top()] > nums[i]) {
            answer[stk.top()] = i;
            stk.pop();
        }
        stk.push(i);
    }
    return answer;
}

// ---- checks: keep these unchanged ----
int main() {
    using V = std::vector<int>;
    CHECK_EQ(nextSmallerRight(V{2, 1, 5, 6, 2, 3}), (V{1, 6, 4, 4, 6, 6}));
    CHECK_EQ(nextSmallerRight(V{}), (V{}));
    CHECK_EQ(nextSmallerRight(V{7}), (V{1}));
    CHECK_EQ(nextSmallerRight(V{3, 3, 3}), (V{3, 3, 3}));
    CHECK_EQ(nextSmallerRight(V{1, 2, 3}), (V{3, 3, 3}));
    CHECK_EQ(nextSmallerRight(V{5, 4, 3, 2, 1}), (V{1, 2, 3, 4, 5}));
    CHECK_EQ(nextSmallerRight(V{3, 3, 2}), (V{2, 2, 3}));
    CHECK_EQ(nextSmallerRight(V{5, 3, 4, 2}), (V{1, 3, 3, 4}));
    CHECK_EQ(nextSmallerRight(V{0, -2, -1, -3}), (V{1, 3, 3, 4}));
    return check::report();
}
