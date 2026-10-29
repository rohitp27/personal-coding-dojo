// Problem: K Closest Points to the Origin
// A Point is (x, y). Return k input points closest to (0, 0).
// Compare squared distances; for equal distances prefer smaller x, then smaller y.
// Duplicate points count separately. Output order does not matter.
// Do not modify the input. k=0 returns empty; invalid k throws std::out_of_range.
// Constraints: coordinates are in [-1,000,000,000, 1,000,000,000].
// Use wide arithmetic BEFORE multiplying coordinates.
//
// Attempt 1: sort a copy. Target O(n log n) time, O(n) extra space.
// Attempt 2: bounded heap. Target O(n log k) time for k>=2 (O(n) for k<=1),
// O(k) extra space, including the output. Do not sort/copy the entire input here.
// Write what your comparator means and what the heap root represents.
// Run: make run FILE=practice/cpp/heaps/04_k_closest_points.cpp

#include "check.h"
#include <algorithm>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

using Point = std::pair<int, int>; // (x, y)
using Points = std::vector<Point>;

long long squaredDistance(const Point& point) {
    return (long long) point.first * point.first + (long long) point.second * point.second;
}

bool compare(const Point& a, const Point& b) {
    long long adist = squaredDistance(a);
    long long bdist = squaredDistance(b);
    if (adist == bdist) {
        if(a.first == b.first) {
            return a.second < b.second;
        }
        return a.first < b.first;
    }
    return adist < bdist;
}

bool compareReverse(const Point& a, const Point& b){
    return compare(b,a);
}

Points kClosestSort([[maybe_unused]] const Points& points,
                    [[maybe_unused]] int k) {
    if (k < 0 || (size_t) k > points.size()) throw std::out_of_range("bad k");
    if (k == 0) return {};
    if (points.empty()) return {};
    Points copyPoints(points.begin(), points.end());
    std::sort(copyPoints.begin(), copyPoints.end(), compareReverse);
    Points answer;
    for(int taken = 0; taken < k; taken++) {
        answer.push_back(copyPoints[points.size()-1-taken]);
    }
    // for(int i = points.size()-1; i>=points.size()-k && i>=0; i--) {
    //     answer.push_back(copyPoints[i]);
    // }
    return answer;
}

Points kClosestHeap(const Points& points, int k) {
    if (k < 0 || (size_t) k > points.size()) throw std::out_of_range("bad k");
    if (k == 0) return {};
    if (points.empty()) return {};
    std::priority_queue<Point, Points, decltype(&compare)> maxHeap(&compare);
    for(size_t i = 0; i < points.size(); i++) {
        if (maxHeap.size() == (size_t) k ) { // full and can replace
            if (compare(points[i], maxHeap.top())) {
                maxHeap.pop();
                maxHeap.push(points[i]);
            }
        } else {
            maxHeap.push(points[i]);
        }
    }
    Points answer;
    while(!maxHeap.empty()) {
        answer.push_back(maxHeap.top());
        maxHeap.pop();
    }
    return answer;
}

// ---- checks: keep these unchanged ----
using Solver = Points (*)(const Points&, int);

static void checkResult(Solver solve, const Points& points, int k, Points expected) {
    const auto original = points;
    auto actual = solve(points, k);
    std::sort(actual.begin(), actual.end());
    std::sort(expected.begin(), expected.end());
    CHECK_EQ(actual, expected);
    CHECK_EQ(points, original);
}

static void checkInvalid(Solver solve, const Points& points, int k) {
    bool threw = false;
    try { (void)solve(points, k); }
    catch (const std::out_of_range&) { threw = true; }
    CHECK(threw);
}

static void runChecks(Solver solve) {
    checkResult(solve, {{1, 3}, {-2, 2}}, 1, {{-2, 2}});
    checkResult(solve, {{3, 3}, {5, -1}, {-2, 4}}, 2, {{3, 3}, {-2, 4}});
    checkResult(solve, {{0, 0}, {1, 1}, {-1, -1}}, 1, {{0, 0}});
    checkResult(solve, {{1, 0}, {0, 1}, {0, -1}, {-1, 0}}, 2,
                {{-1, 0}, {0, -1}});
    checkResult(solve, {{2, 0}, {1, 1}, {1, 1}, {3, 0}}, 2,
                {{1, 1}, {1, 1}});
    checkResult(solve, {{7, -4}}, 1, {{7, -4}});
    checkResult(solve, {}, 0, {});
    checkResult(solve, {{1, 2}}, 0, {});
    checkResult(solve, {{3, 4}, {-1, 0}, {0, 0}}, 3,
                {{3, 4}, {-1, 0}, {0, 0}});
    checkResult(solve, {{1000000000, 1000000000}, {-1000000000, 0},
                        {50000, 50000}, {0, 0}}, 3,
                {{-1000000000, 0}, {50000, 50000}, {0, 0}});
    checkInvalid(solve, {}, 1);
    checkInvalid(solve, {{1, 2}}, -1);
    checkInvalid(solve, {{1, 2}}, 2);
}

int main() {
    std::cout << "Sorting approach:\n";
    runChecks(kClosestSort);
    std::cout << "Heap approach:\n";
    runChecks(kClosestHeap);
    return check::report();
}
