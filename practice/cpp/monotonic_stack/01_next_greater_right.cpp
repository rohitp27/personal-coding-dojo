// Problem: Next Greater Element to the Right
// For each position, return the value of the first strictly greater element
// to its right, or -1 if none exists. Equal values do not count as greater.
// Example: {2, 1, 2, 4, 3} -> {4, 2, 4, -1, -1}
// Do not modify nums. Empty input returns an empty vector.
// Target: O(n) time, O(n) extra space. Values may be negative.
//
// Before coding, write your stack invariant and explain why the nested loops
// still take O(n) total time.
// Run: make run FILE=practice/cpp/monotonic_stack/01_next_greater_right.cpp

#include "check.h"
#include <stack>
#include <vector>

std::vector<int> nextGreaterRight(const std::vector<int>& nums) {
    std::vector<int> answer(nums.size(), -1);
    std::stack<int> stk;
    for(size_t i = 0; i < nums.size(); i++) {
        size_t current_index = i;
        while(!stk.empty() && nums[stk.top()] < nums[current_index]) {
            answer[stk.top()] = nums[current_index];
            stk.pop();
        }
        stk.push(current_index);
    }
    return answer;
}

// ---- checks: keep these unchanged ----
int main() {
    using V = std::vector<int>;
    CHECK_EQ(nextGreaterRight(V{2, 1, 2, 4, 3}), (V{4, 2, 4, -1, -1}));
    CHECK_EQ(nextGreaterRight(V{3, 3, 3}), (V{-1, -1, -1}));
    CHECK_EQ(nextGreaterRight(V{5, 4, 3, 2, 1}), (V{-1, -1, -1, -1, -1}));
    CHECK_EQ(nextGreaterRight(V{}), (V{}));
    CHECK_EQ(nextGreaterRight(V{7}), (V{-1}));
    CHECK_EQ(nextGreaterRight(V{1, 2, 3, 4}), (V{2, 3, 4, -1}));
    CHECK_EQ(nextGreaterRight(V{2, 2, 3}), (V{3, 3, -1}));
    CHECK_EQ(nextGreaterRight(V{2, 5, 3, 6}), (V{5, 6, 6, -1}));
    CHECK_EQ(nextGreaterRight(V{-3, -2, 0}), (V{-2, 0, -1}));
    const V input{4, 1, 5};
    CHECK_EQ(nextGreaterRight(input), (V{5, 5, -1}));
    CHECK_EQ(input, (V{4, 1, 5}));
    return check::report();
}
