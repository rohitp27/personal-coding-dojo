// Problem: Three small std::stack exercises.
//   isBalanced(s)               true if every (, [, { in s is closed by the matching
//                               bracket in the right order; other characters are ignored
//   maxDepth(s)                 deepest nesting of '(' in a balanced string made of only
//                               '(' and ')'; 0 for ""
//   removeAdjacentDuplicates(s) repeatedly delete two equal neighbouring characters until
//                               none remain
//
// Examples:
//   isBalanced("a(b[c]{d})")          -> true
//   isBalanced("([)]")                -> false
//   maxDepth("(()(()))")              -> 3
//   removeAdjacentDuplicates("abbaca")-> "ca"     (abbaca -> aaca -> ca)
//
// Edge cases: empty string, lone closer ")", lone opener "(", leftover openers.
//
// Run with: make run FILE=practice/cpp/stacks_queues/03_balanced_brackets.cpp
//
// Syntax reminder:
//   std::stack<char> st;  st.push(c);  st.top();  st.pop();  st.empty();
//   // check st.empty() BEFORE top()/pop(); both are undefined behaviour on an empty stack
//   switch (c) { case ')': ... break; }     // or: if (c == ')' && ...)
//   std::string out;  out.push_back(c);  out.back();  out.pop_back();   // a string works as a stack

#include <stack>
#include <string>

#include "check.h"

bool isBalanced(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char open = st.top();
            st.pop();
            if ((c == ')' && open != '(') || (c == ']' && open != '[') || (c == '}' && open != '{')) {
                return false;
            }
        }
    }
    return st.empty();
}

int maxDepth(const std::string& s) {
    int depth = 0, best = 0;
    for (char c : s) {
        if (c == '(') {
            depth++;
            if (depth > best) best = depth;
        } else if (c == ')') {
            depth--;
        }
    }
    return best;
}

std::string removeAdjacentDuplicates(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (!out.empty() && out.back() == c) {
            out.pop_back();
        } else {
            out.push_back(c);
        }
    }
    return out;
}

int main() {
    CHECK(isBalanced(""));
    CHECK(isBalanced("()"));
    CHECK(isBalanced("([]{})"));
    CHECK(isBalanced("a(b[c]{d})"));
    CHECK(!isBalanced("(]"));
    CHECK(!isBalanced("(("));
    CHECK(!isBalanced(")"));
    CHECK(!isBalanced("([)]"));
    CHECK(!isBalanced("(()"));

    CHECK_EQ(maxDepth(""), 0);
    CHECK_EQ(maxDepth("()"), 1);
    CHECK_EQ(maxDepth("()()"), 1);
    CHECK_EQ(maxDepth("(()(()))"), 3);

    CHECK_EQ(removeAdjacentDuplicates(""), "");
    CHECK_EQ(removeAdjacentDuplicates("a"), "a");
    CHECK_EQ(removeAdjacentDuplicates("aa"), "");
    CHECK_EQ(removeAdjacentDuplicates("abbaca"), "ca");
    CHECK_EQ(removeAdjacentDuplicates("abba"), "");

    return check::report();
}
