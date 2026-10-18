// Problem: Balanced Brackets
// Given a string made only of the characters ( ) [ ] { }, return true if every opening
// bracket is closed by the same kind of bracket, in the correct order. The empty string
// is balanced.
//
// Examples:
//   "()[]{}" -> true
//   "([{}])" -> true
//   "(]"     -> false
//   "([)]"   -> false   (closes in the wrong order)
//   "(("     -> false   (left open)
//   "))"     -> false   (closes with nothing open)
//
// Constraints: 0 <= s.size() <= 1,000,000.
// Required complexity: O(n) time, O(n) space.
//
// Hint: scan left to right with a stack of the opening brackets seen so far. On a closing
// bracket, the stack must be non-empty and its top must be the matching opener. What must
// be true of the stack when the scan ends?
//
// Run: make run FILE=curriculum/102_fundamentals/stacks_queues_lists/01_balanced_brackets.cpp

#include "check.h"

#include <string>
#include <vector>

bool isBalanced([[maybe_unused]] const std::string& s) {
    // TODO
    return false;
}

int main() {
    CHECK_EQ(isBalanced(""), true);
    CHECK_EQ(isBalanced("()"), true);
    CHECK_EQ(isBalanced("()[]{}"), true);
    CHECK_EQ(isBalanced("([{}])"), true);
    CHECK_EQ(isBalanced("{[]}"), true);
    CHECK_EQ(isBalanced("(()())"), true);
    CHECK_EQ(isBalanced("(]"), false);
    CHECK_EQ(isBalanced("([)]"), false);
    CHECK_EQ(isBalanced("(("), false);
    CHECK_EQ(isBalanced("))"), false);
    CHECK_EQ(isBalanced("("), false);
    CHECK_EQ(isBalanced("]"), false);
    CHECK_EQ(isBalanced(")("), false);
    CHECK_EQ(isBalanced("{[}]"), false);
    CHECK_EQ(isBalanced("(()"), false);
    CHECK_EQ(isBalanced("())"), false);
    CHECK_EQ(isBalanced("{{{{}}}}[]"), true);

    std::string deepOpen(500000, '('), deepClose(500000, ')');
    CHECK_EQ(isBalanced(deepOpen + deepClose), true);
    CHECK_EQ(isBalanced(deepOpen + deepClose.substr(1)), false);
    CHECK_EQ(isBalanced(deepOpen.substr(1) + deepClose), false);
    std::string pairs;
    for (int i = 0; i < 500000; i++) pairs += "()";
    CHECK_EQ(isBalanced(pairs), true);
    CHECK_EQ(isBalanced(std::string(500000, '(') + std::string(500000, ']')), false);

    return check::report();
}
