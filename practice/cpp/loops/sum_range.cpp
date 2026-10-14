// Problem: Write sumRange(n) that returns the sum of integers 1..n using a for loop
// (no closed-form formula). sumRange(0) should return 0.
//
// Run with: make run FILE=practice/cpp/loops/sum_range.cpp

#include <cassert>
#include <iostream>
#include <vector>

int sumRange(int n) {
    // TODO: implement using a for loop
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += (n-i);
        //for n=5; i=0, sum=5; i = 1, sum=5+5-1=9;... ; i = 4, sum=14 + (5-4)=14+1=15
    }
    return sum;
}

int main() {
    assert(sumRange(0) == 0);
    assert(sumRange(1) == 1);
    assert(sumRange(5) == 15);
    assert(sumRange(10) == 55);

    std::cout << "all tests passed\n";
    return 0;
}
