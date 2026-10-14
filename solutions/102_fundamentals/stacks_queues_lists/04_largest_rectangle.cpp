// Problem: Largest Rectangle in a Histogram (monotonic stack)
// Given bar heights of a histogram (each bar has width 1), return the area of the largest
// axis-aligned rectangle that fits entirely inside the bars (its height is limited by the
// shortest bar it spans). An empty histogram has area 0.
//
// Examples:
//   {2,1,5,6,2,3} -> 10   (the bars 5 and 6, height 5, width 2)
//   {2,4}         -> 4
//   {3,3,3}       -> 9
//   {0,0}         -> 0
//
// Constraints: 0 <= h.size() <= 200,000; 0 <= height <= 1,000,000,000.
//              The area can exceed 32 bits.
// Required complexity: O(n) time, O(n) space. Expanding left and right from every bar is
// O(n^2) and too slow for the large tests.
//
// Think first:
//   1. For a bar of height h[i] taken as the limiting (shortest) bar, which two positions
//      bound the widest rectangle that uses it, and how do they relate to "the nearest
//      smaller bar to the left / to the right"?
//   2. If you keep a stack of bar indices, what ordering must the stack maintain, and what
//      does popping a bar tell you at that exact moment?
//   3. How do you flush the bars still on the stack at the end, and what is the width of a
//      rectangle popped when the stack becomes empty?
//
// Run: make run FILE=curriculum/102_fundamentals/stacks_queues_lists/04_largest_rectangle.cpp

#include "check.h"

#include <algorithm>
#include <vector>

long long largestRectangle(const std::vector<int>& h) {
    std::vector<int> st;
    long long best = 0;
    int n = static_cast<int>(h.size());
    for (int i = 0; i <= n; i++) {
        int cur = (i == n) ? 0 : h[i];
        while (!st.empty() && h[st.back()] >= cur) {
            long long height = h[st.back()];
            st.pop_back();
            long long left = st.empty() ? -1 : st.back();
            best = std::max(best, height * (i - left - 1));
        }
        st.push_back(i);
    }
    return best;
}

int main() {
    CHECK_EQ(largestRectangle({2, 1, 5, 6, 2, 3}), 10LL);
    CHECK_EQ(largestRectangle({2, 4}), 4LL);
    CHECK_EQ(largestRectangle({}), 0LL);
    CHECK_EQ(largestRectangle({0}), 0LL);
    CHECK_EQ(largestRectangle({5}), 5LL);
    CHECK_EQ(largestRectangle({3, 3, 3}), 9LL);
    CHECK_EQ(largestRectangle({1, 2, 3, 4, 5}), 9LL);  // height 3 over the last three bars
    CHECK_EQ(largestRectangle({5, 4, 3, 2, 1}), 9LL);
    CHECK_EQ(largestRectangle({0, 0, 0}), 0LL);
    CHECK_EQ(largestRectangle({6, 2, 5, 4, 5, 1, 6}), 12LL);
    CHECK_EQ(largestRectangle({2, 1, 2}), 3LL);  // height 1 across all three
    CHECK_EQ(largestRectangle({4, 0, 4}), 4LL);  // a zero bar splits the histogram
    CHECK_EQ(largestRectangle({1, 1, 0, 1, 1, 1}), 3LL);
    CHECK_EQ(largestRectangle({1000000000, 1000000000}), 2000000000LL);  // overflows int

    // Large inputs: O(n^2) expansion is ~4*10^10 steps.
    {
        int n = 200000;
        CHECK_EQ(largestRectangle(std::vector<int>(n, 7)), 1400000LL);
        std::vector<int> asc(n), desc(n);
        for (int i = 0; i < n; i++) {
            asc[i] = i + 1;
            desc[i] = n - i;
        }
        // best is height 100000 (or 100001) over about half the bars: 100000 * 100001
        CHECK_EQ(largestRectangle(asc), 10000100000LL);
        CHECK_EQ(largestRectangle(desc), 10000100000LL);
        CHECK_EQ(largestRectangle(std::vector<int>(n, 1000000000)), 200000000000000LL);
    }

    return check::report();
}
