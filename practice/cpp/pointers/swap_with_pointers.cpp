// Problem: Write swapInts(int* a, int* b) that swaps the values pointed to by a and b,
// using raw pointers (no std::swap).
//
// Run with: make run FILE=practice/cpp/pointers/swap_with_pointers.cpp

#include <cassert>
#include <iostream>

void swapInts(int* a, int* b) { // (a, b)
    // TODO: implement using pointer dereferences
    *b = *b + *a; // a, b+a;
    *a = *b - *a; // b, b+a; 
    *b = *b - *a; // b, a;
}

int main() {
    int x = 1;
    int y = 2;
    swapInts(&x, &y);
    assert(x == 2);
    assert(y == 1);

    std::cout << "all tests passed\n";
    return 0;
}
