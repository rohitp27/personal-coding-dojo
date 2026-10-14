// Problem: Write sumArray(const int* arr, int n) that returns the sum of n elements,
// walking the array using pointer arithmetic (e.g. *(arr + i), or incrementing a
// pointer) rather than arr[i] indexing.
//
// Run with: make run FILE=practice/cpp/pointers/array_sum_pointer.cpp

#include <cassert>
#include <iostream>

int sumArray(const int* arr, int n) {
    // TODO: implement using pointer arithmetic, not arr[i]
    int sum = 0;
    for (int i = 0; i < n; i++){
        sum += *(arr+i);
    }
    return sum;
}

int main() {
    int a[] = {1, 2, 3, 4, 5};
    assert(sumArray(a, 5) == 15);

    int b[] = {0};
    assert(sumArray(b, 1) == 0);

    int empty[] = {};
    assert(sumArray(empty, 0) == 0);

    std::cout << "all tests passed\n";
    return 0;
}
