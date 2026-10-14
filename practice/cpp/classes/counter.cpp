// Problem: Implement a Counter class.
//   - Default constructor starts the count at 0.
//   - Constructor overload takes a starting int value.
//   - increment() adds 1 and returns the new value.
//   - decrement() subtracts 1 and returns the new value.
//   - value() returns the current count.
//   - reset() sets the count back to 0.
//
// This exercises constructor overloading, not just a single constructor.
//
// Run with: make run FILE=practice/cpp/classes/counter.cpp

#include <cassert>
#include <iostream>

class Counter {
public:
    // TODO: default constructor
    Counter() : Counter(0) {}

    // TODO: constructor(int start)
    Counter(int start) {
        assert(start >= 0);
        count = start;
    }

    // TODO: int increment()
    int increment() {
        return ++count;
    }

    // TODO: int decrement()
    int decrement() {
        return --count;
    }

    // TODO: int value() const
    int value() const {
        return count;
    }

    // TODO: void reset()
    void reset() {
        count = 0;
    }

private:
    // TODO: member variable
    int count;
};

int main() {
    Counter c;
    assert(c.value() == 0);
    assert(c.increment() == 1);
    assert(c.increment() == 2);
    assert(c.decrement() == 1);
    c.reset();
    assert(c.value() == 0);

    Counter c2(10);
    assert(c2.value() == 10);
    assert(c2.increment() == 11);

    std::cout << "all tests passed\n";
    return 0;
}
