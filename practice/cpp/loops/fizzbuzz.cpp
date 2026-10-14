// Problem: Write fizzbuzz(n) that returns a vector<string> of length n where,
// for i = 1..n:
//   - multiples of 3 and 5 -> "FizzBuzz"
//   - multiples of 3       -> "Fizz"
//   - multiples of 5       -> "Buzz"
//   - otherwise            -> the number itself, as a string
//
// Run with: make run FILE=practice/cpp/loops/fizzbuzz.cpp

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

std::string FIZZ = "Fizz";
std::string BUZZ = "Buzz";

std::vector<std::string> fizzbuzz(int n) {
    std::vector<std::string> result(n);
    for (int i = 0; i < n; i++){
        int val = i+1;
        std::string response = "";
        if (! (val%3==0) && !(val%5==0) ){
            response = std::to_string(val);
        } else {
            response = "";
            if (val%3==0) {
                response += FIZZ;
            }
            if (val%5==0) {
                response += BUZZ;
            }
        }
        result.at(i) = response;
    }
    return result;
}

int main() {
    auto result = fizzbuzz(15);
    std::vector<std::string> expected = {
        "1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8",
        "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz"
    };
    assert(result == expected);

    std::cout << "all tests passed\n";
    return 0;
}
