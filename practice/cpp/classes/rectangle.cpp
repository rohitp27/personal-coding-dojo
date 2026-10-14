// Problem: Implement a Rectangle class.
//   - Constructor takes width and height (doubles).
//   - area() returns width * height.
//   - perimeter() returns 2 * (width + height).
//   - isSquare() returns true if width == height.
//
// Run with: make run FILE=practice/cpp/classes/rectangle.cpp

#include <cassert>
#include <iostream>

class Rectangle {
public:
    // TODO: constructor(double width, double height)
    Rectangle(double width, double height) {
        assert(width > 0);
        assert(height > 0);
        _width = width;
        _height = height;
    }

    // TODO: double area() const
    double area() const {
        return _width * _height;
    }

    // TODO: double perimeter() const
    double perimeter() const {
        return 2*(_width + _height);
    }

    // TODO: bool isSquare() const
    bool isSquare() const {
        return _width == _height;
    }


private:
    // TODO: member variables
    double _width;
    double _height;
};

int main() {
    Rectangle r(3.0, 4.0);
    assert(r.area() == 12.0);
    assert(r.perimeter() == 14.0);
    assert(!r.isSquare());

    Rectangle sq(5.0, 5.0);
    assert(sq.isSquare());

    std::cout << "all tests passed\n";
    return 0;
}
