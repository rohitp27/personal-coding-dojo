// Problem: Sort a vector<Person> using std::sort with a custom comparator, two ways:
//   1. sortByAgeAsc: a free function comparator, sorts by age ascending.
//   2. sortByNameThenAgeDesc: a lambda passed inline to std::sort, sorting by name
//      alphabetically, and for equal names, by age descending.
//
// A comparator must return true if the first argument should come strictly before
// the second (i.e. it behaves like "a < b" for the sort order you want).
//
// Run with: make run FILE=practice/cpp/sorting/custom_comparator.cpp

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

struct Person {
    std::string name;
    int age;

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// TODO: bool comparator function `bool byAgeAsc(const Person& a, const Person& b)`
// returning true if a.age < b.age

bool byAgeAsc(const Person& a, const Person& b) {
    return a.age < b.age;
}

void sortByAgeAsc(std::vector<Person>& people) {
    // TODO: std::sort(people.begin(), people.end(), byAgeAsc);
    std::sort(people.begin(), people.end(), byAgeAsc);
}

void sortByNameThenAgeDesc(std::vector<Person>& people) {
    // TODO: std::sort with an inline lambda:
    //   - primary key: name ascending
    //   - tiebreaker: age descending
    std::sort(
        people.begin(), 
        people.end(), 
        [](const Person& a, const Person& b){
            return (a.name == b.name) ? a.age > b.age : a.name < b.name;
        }
    );
}

int main() {
    std::vector<Person> people = {
        {"Charlie", 25}, {"Alice", 30}, {"Bob", 20}
    };
    sortByAgeAsc(people);
    std::vector<Person> expectedByAge = {
        {"Bob", 20}, {"Charlie", 25}, {"Alice", 30}
    };
    assert(people == expectedByAge);

    std::vector<Person> people2 = {
        {"Alice", 30}, {"Bob", 20}, {"Alice", 25}
    };
    sortByNameThenAgeDesc(people2);
    std::vector<Person> expectedByNameAge = {
        {"Alice", 30}, {"Alice", 25}, {"Bob", 20}
    };
    assert(people2 == expectedByNameAge);

    std::cout << "all tests passed\n";
    return 0;
}
