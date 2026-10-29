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
// Run: make run FILE=practice/cpp/stacks_queues_lists/04_largest_rectangle.cpp

#include "check.h"

#include <algorithm>
#include <vector>
#include <assert.h>

std::vector<size_t> smallerToRight(const std::vector<int>& h) {
    std::vector<size_t> answer(h.size(), __SIZE_MAX__);
    std::stack<size_t> stk;
    for(size_t i = 0; i < h.size(); i++) {
        while( !stk.empty() && h[stk.top()] > h[i] ) {
            answer[stk.top()] = i;
            stk.pop();
        }
        stk.push(i);
    }
    return answer;
}

std::vector<size_t> smallerToLeft(const std::vector<int>& h) {
    std::vector<size_t> answer(h.size(), __SIZE_MAX__);
    std::stack<size_t> stk;
    size_t i;
    for(i = h.size()-1; i != 0; i--) {
        while( !stk.empty() && h[stk.top()] > h[i] ) {
            answer[stk.top()] = i;
            stk.pop();
        }
        stk.push(i);
    }
    // for i == 0
    assert(i == 0);
    while(!stk.empty() && h[stk.top()] > h[i] ) {
        answer[stk.top()] = i;
        stk.pop();
    }
    // no need to pop
    return answer;
}

size_t max(size_t a, size_t b) {
    return (a<b)?b:a;
}

size_t min(size_t a, size_t b) {
    return (a<b)?a:b;
}

long long barsToRight(size_t i, size_t rightBoundary, size_t arraySize) {
    assert (i < rightBoundary);
    // std::cout<<"| i : "<<i<<", rBoundary : "<<rightBoundary<<", Size : "<<arraySize<<", Min:"<<min(rightBoundary, arraySize)<<std::endl;
    return min(rightBoundary, arraySize) - i - 1;
}

long long barsToLeft(size_t i, size_t leftBoundary) {
    if (leftBoundary == __SIZE_MAX__) {
        // nothing on left
        return i;
    }
    assert (leftBoundary < i);
    // printf("i %d, leftBoundary %d\n", i, leftBoundary);
    return i - leftBoundary - 1;
}

void printBars(std::string output, std::vector<size_t> boundaries) {
    std::cout<<output;
    for(size_t b : boundaries) {
        std::cout<<" | "<<b;
    }
    std::cout<<std::endl;
}

long long largestRectangle([[maybe_unused]] const std::vector<int>& h) {
    if (h.empty()) return 0;
    if (h.size() == 1) return h.front();
    std::vector<size_t> leftBoundary = smallerToLeft(h);
    std::vector<size_t> rightBoundary = smallerToRight(h);
    // printBars("Left boundaries", leftBoundary);
    // printBars("Right boundaries", rightBoundary);
    long long largestArea = 0;
    for(size_t i = 0; i < h.size(); i++) {
        long long height = h[i];
        if (height == 0) continue;
        long long width = 1 + barsToLeft(i, leftBoundary[i]) + barsToRight(i, rightBoundary[i], h.size());
        // assert(width > 0);
        long long area = width * height;
        // std::cout<<"h "<<height<<" w "<<width<<" area "<<area<<std::endl;
        largestArea = max(area, largestArea);
    }
    return largestArea;
}

// Second attempt: calculate areas during one stack traversal, without boundary arrays.
// Same contract as largestRectangle. Target: O(n) time, O(n) extra space.
long long largestRectangleSinglePass(const std::vector<int>& h) {
    if (h.empty()) return 0;
    std::vector<size_t> answers(h.size(), __SIZE_MAX__);
    std::stack<size_t> stk;
    long long maxArea = 0;
    for(size_t i = 0; i < h.size(); i++) {
        while(!stk.empty() && h[stk.top()] > h[i]) {
            size_t top = stk.top();
            stk.pop();
            
            long long rightBoundary = i;
            long long leftBoundary; 
            if (stk.empty()) {
                //unbounded to left
                leftBoundary = -1;
            } else {
                leftBoundary = stk.top();
            }
            long long width = rightBoundary-leftBoundary-1;
            int height = h[top];
            maxArea = max(maxArea, height * width);
        }
        stk.push(i);
    }
    while(!stk.empty()) {
        // these are unbounded to the right
        size_t top = stk.top();
        stk.pop();
        long long width;
        if (stk.empty()) {
            // nothing to it's left either
            width = h.size();
        } else {
            width = h.size() - top;
        }
        long long height = h[top];
        maxArea = max(maxArea, height*width);
    }
    return maxArea;
}

// Run the same checks against either implementation.
void checkRectangles(long long (*largestRectangle)(const std::vector<int>&)) {
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

}

int main() {
    std::cout << "Two-array implementation:\n";
    checkRectangles(largestRectangle);
    std::cout << "Single-pass implementation:\n";
    checkRectangles(largestRectangleSinglePass);
    return check::report();
}
