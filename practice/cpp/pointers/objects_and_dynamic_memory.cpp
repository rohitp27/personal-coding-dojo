// Problem: Practice dynamic allocation of objects (not just ints).
//   - Allocate a Rectangle on the heap with `new Rectangle(w, h)`, giving you a
//     Rectangle*.
//   - Access its methods/members via `->` (not `.`) since you have a pointer.
//   - Free it with `delete` when done.
//   - Implement makeRectangles(n), which heap-allocates n Rectangle objects
//     (each with width = i+1, height = i+2, for i in 0..n-1), returning a
//     vector<Rectangle*>.
//   - Implement totalArea, which sums area() over a vector<Rectangle*>
//     (using -> to call area() through each pointer).
//   - Implement freeRectangles, which deletes every pointer in the vector
//     (each `new` must be matched with a `delete`, or it's a memory leak).
//
// Run with: make run FILE=practice/cpp/pointers/objects_and_dynamic_memory.cpp

#include <cassert>
#include <iostream>
#include <vector>

class Rectangle {
public:
    Rectangle(double width, double height) : width_(width), height_(height) {}
    double area() const { return width_ * height_; }

private:
    double width_;
    double height_;
};

std::vector<Rectangle*> makeRectangles(int n) {
    std::vector<Rectangle*> rects;
    // TODO: for i in 0..n-1, `new Rectangle(i + 1, i + 2)` and push_back the pointer
    return rects;
}

double totalArea(const std::vector<Rectangle*>& rects) {
    double total = 0.0;
    // TODO: sum rect->area() for each pointer in rects
    return total;
}

void freeRectangles(std::vector<Rectangle*>& rects) {
    // TODO: delete each pointer, then clear() the vector
}

int main() {
    std::vector<Rectangle*> rects = makeRectangles(3);
    assert(rects.size() == 3);

    // rect 0: 1x2 = 2, rect 1: 2x3 = 6, rect 2: 3x4 = 12 -> total 20
    assert(totalArea(rects) == 20.0);

    freeRectangles(rects);
    assert(rects.empty());

    std::cout << "all tests passed\n";
    return 0;
}
