// Problem: Implement insertion sort by hand (no std::sort, no other library sort).
//   insertionSort(vector<int>& arr) sorts arr in place, ascending.
//
// Classic whiteboard ask — walk left to right, take each element, and shift it
// backward through the already-sorted prefix until it lands in the right spot.
//
// Run with: make run FILE=practice/cpp/sorting/implement_sort.cpp

#include <cassert>
#include <iostream>
#include <vector>

void insertionSort(std::vector<int>& arr) {
    int start=0;

    for (int end = 0; end + 1 < (int)arr.size(); end++) {
        int key = arr[end+1];
        int insert_index = 0;
        // find insert_index
        for (int compare_index = end; compare_index >= start; compare_index--) {
            if (arr[compare_index] < key) {
                insert_index = compare_index + 1;
                break;
            }
        }
        // shift right and insert
        for (int current = end; current >= insert_index; current--) {
            arr[current+1] = arr[current];
        }
        arr[insert_index] = key;
    }
}

int main() {
    std::vector<int> a = {5, 2, 4, 6, 1, 3};
    insertionSort(a);
    assert((a == std::vector<int>{1, 2, 3, 4, 5, 6}));

    std::vector<int> b = {};
    insertionSort(b);
    assert(b.empty());

    std::vector<int> c = {1};
    insertionSort(c);
    assert((c == std::vector<int>{1}));

    std::vector<int> d = {3, 3, 1, 2, 1};
    insertionSort(d);
    assert((d == std::vector<int>{1, 1, 2, 3, 3}));

    std::cout << "all tests passed\n";
    return 0;
}
