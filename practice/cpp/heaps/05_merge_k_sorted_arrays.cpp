// Problem: Merge K Sorted Arrays
// Given arrays individually sorted in ascending order, return all their elements
// in one ascending vector. Preserve duplicates and do not modify the inputs.
// Empty input and empty inner arrays are allowed.
// Use a priority_queue holding at most one next candidate per nonempty array.
// Define your own Entry struct and comparator below.
// Target: O(K + N log(K+1)) time, O(K) auxiliary space plus O(N) output,
// where K is the number of arrays and N is the total number of elements.
// Scanning the K arrays for initialization is necessary even if all are empty.
// Example: {{1,4,7}, {2,5}, {3,6,8}} -> {1,2,3,4,5,6,7,8}
// Run: make run FILE=practice/cpp/heaps/05_merge_k_sorted_arrays.cpp

#include "check.h"
#include <limits>
#include <queue>
#include <vector>
#include <assert.h>
#include <optional>

using Arrays = std::vector<std::vector<int>>;

// Write your Entry struct and comparator here.
class Merger {
    struct Entry {
        int value;
        size_t listIndex;
        size_t elementIndex;
    };

    struct Compare {
        bool operator()(const Entry& a, const Entry& b) const {
            if (a.value == b.value) return a.listIndex > b.listIndex;
            return a.value > b.value;
        }
    };

    public:
        explicit Merger(const Arrays& _arrays) : arrays(_arrays) {}

        std::vector<int> sortedList(){
            std::vector<int> answer;
            // initialize
            initialize();
            // loop until heap is empty
            while(!done()) {
                answer.push_back(popHeap());
            }
            return answer;
        }
    private:
        std::priority_queue<Entry, std::vector<Entry>, Compare> minHeap;
        const Arrays& arrays;

        bool done() {
            return minHeap.empty();
        }

        int popHeap() {
            assert(!minHeap.empty());
            // pop the heap
            int answer = minHeap.top().value;
            // add a new element and pop
            auto next = getNext(minHeap.top());
            minHeap.pop();
            if (next.has_value()) {
                minHeap.push(next.value());
            }
            return answer;
        }

        std::optional<Entry> getNext(const Entry& oldEntry) {
            int previousVal = oldEntry.value;
            size_t listIndex = oldEntry.listIndex;
            size_t oldElementIndex = oldEntry.elementIndex;

            assert(arrays[listIndex][oldElementIndex] == previousVal);

            if (oldElementIndex+1 >= arrays[listIndex].size()) {
                // this is the end;
                return std::nullopt;
            } else {
                // valid index
                size_t newElementIndex = oldElementIndex+1;
                int newVal = arrays[listIndex][newElementIndex];
                return Entry{newVal, listIndex, newElementIndex};
            }
        }

        bool initialize() {
            if (arrays.empty()) {
                return false;
            }
            assert(minHeap.empty());
            assert(!arrays.empty());
            for (size_t i = 0; i < arrays.size(); i++) {
                if (arrays[i].empty()) {
                    continue;
                }
                // create element for this
                assert(!arrays[i].empty());
                size_t listIndex = i;
                size_t elementIndex = 0; // because this is init
                int value = arrays[i][elementIndex];
                Entry toPush{value, listIndex, elementIndex};
                minHeap.push(toPush);
            }
            return true;
        }
};


std::vector<int> mergeKSorted(const std::vector<std::vector<int>>& arrays) {
    Merger m{arrays};
    return m.sortedList();
}

// ---- checks: keep these unchanged ----
static void checkMerge(const std::vector<std::vector<int>>& arrays,
                       const std::vector<int>& expected) {
    const auto original = arrays;
    CHECK_EQ(mergeKSorted(arrays), expected);
    CHECK(arrays == original);
}

int main() {
    checkMerge({{1, 4, 7}, {2, 5}, {3, 6, 8}}, {1, 2, 3, 4, 5, 6, 7, 8});
    checkMerge({}, {});
    checkMerge({{}, {}, {}}, {});
    checkMerge({{}, {2}, {}, {1, 3}, {}}, {1, 2, 3});
    checkMerge({{1, 2, 3}}, {1, 2, 3});
    checkMerge({{3}, {1}, {2}}, {1, 2, 3});
    checkMerge({{1, 1, 4}, {1, 2, 4}, {1}}, {1, 1, 1, 1, 2, 4, 4});
    checkMerge({{-9, -3, 0}, {-8, -3, 2}, {-1}}, {-9, -8, -3, -3, -1, 0, 2});
    checkMerge({{1}, {2, 3, 4, 5, 6}, {7}}, {1, 2, 3, 4, 5, 6, 7});
    const int lo = std::numeric_limits<int>::min();
    const int hi = std::numeric_limits<int>::max();
    checkMerge({{lo, 0, hi}, {lo, hi}}, {lo, lo, 0, hi, hi});
    std::vector<std::vector<int>> arrays(20);
    for (int i = 0; i < 2000; ++i) arrays[i % 20].push_back(i);
    std::vector<int> expected;
    for (int i = 0; i < 2000; ++i) expected.push_back(i);
    CHECK(mergeKSorted(arrays) == expected);
    return check::report();
}
