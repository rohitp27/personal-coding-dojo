// Problem: Two exercises with std::queue (plus std::stack as a helper).
//   reverseFirstK(q, k)    return q with its first k elements reversed, the rest unchanged
//                          (0 <= k <= q.size())
//   lastRemaining(n, k)    n people stand in a circle numbered 1..n. Starting at 1, count
//                          k people and remove the k-th, continue counting from the next
//                          person, until one is left. Return that person's number.
//
// Examples:
//   reverseFirstK({1,2,3,4,5}, 3)   -> {3,2,1,4,5}
//   lastRemaining(5, 2)             -> 3     (removed in order 2,4,1,5)
//
// Edge cases: k = 0, k = size, empty queue, n = 1.
//
// Run with: make run FILE=practice/cpp/stacks_queues/02_queue_tricks.cpp
//
// Syntax reminder:
//   std::queue<int> q;  q.push(x);  q.front();  q.pop();  q.size();  q.empty();
//   // pop() returns void: read front() first, then pop()
//   std::stack<int> st;  st.push(x);  st.top();  st.pop();
//   // queues are passed/returned by value here, so modifying the parameter is fine
//   // rotate: q.push(q.front()); q.pop();

#include <queue>
#include <stack>
#include <vector>

#include "check.h"

std::queue<int> reverseFirstK(std::queue<int> q, int k) {
    (void)q; (void)k;
    int untouched = q.size() - k;
    int touched = k;
    std::stack<int> st;
    if (q.empty()) return q;
    while (touched > 0 && !q.empty()) {
        int top = q.front();
        st.push(top);
        q.pop();
        touched--;
    }
    while (!st.empty()) {
        int top = st.top();
        q.push(top);
        st.pop();
    }
    while (untouched > 0 && ! q.empty()) {
        q.push(q.front());
        q.pop();
        untouched--;
    }
    return q;
}

int lastRemaining(int n, int k) {
    (void)n; (void)k;
    std::vector<int> v;
    std::vector<int> other;
    for (int i = 0; i < n; i++){
        v.push_back(i);
    }
    int remaining = n;
    int ptr = 0;
    int counter = k;
    while(remaining > 1){
        if (v[ptr] == -1) {
            // was already removed
            ptr = (ptr+1) % n;
            continue;
        } else { //this is an actual number
                    // printf("at %d, ptr %d, ctr %d , remaining %d ->\n", v[ptr], ptr, counter, remaining);

            counter--;
            if (counter == 0) {
                // delete number
                remaining--;
                v[ptr] = -1;
                // printf("\tDelete \n");
                //reset counter
                counter = k;
            }
            // move forward
            ptr = (ptr+1) % n;
        }
    }
    for (int i = 0; i < n; i++) {
        if ( v[i] != -1 ) return v[i]+1;
    }
    return -1;
}

static std::queue<int> makeQueue(const std::vector<int>& v) {
    std::queue<int> q;
    for (int x : v) q.push(x);
    return q;
}

static std::vector<int> drain(std::queue<int> q) {
    std::vector<int> v;
    while (!q.empty()) {
        v.push_back(q.front());
        q.pop();
    }
    return v;
}

int main() {
    CHECK_EQ(drain(reverseFirstK(makeQueue({}), 0)), std::vector<int>{});
    CHECK_EQ(drain(reverseFirstK(makeQueue({1, 2, 3}), 0)), (std::vector<int>{1, 2, 3}));
    CHECK_EQ(drain(reverseFirstK(makeQueue({1, 2, 3}), 1)), (std::vector<int>{1, 2, 3}));
    CHECK_EQ(drain(reverseFirstK(makeQueue({1, 2, 3, 4, 5}), 3)), (std::vector<int>{3, 2, 1, 4, 5}));
    CHECK_EQ(drain(reverseFirstK(makeQueue({1, 2, 3, 4, 5}), 5)), (std::vector<int>{5, 4, 3, 2, 1}));

    CHECK_EQ(lastRemaining(1, 3), 1);
    CHECK_EQ(lastRemaining(5, 2), 3);
    CHECK_EQ(lastRemaining(6, 2), 5);
    CHECK_EQ(lastRemaining(7, 3), 4);
    CHECK_EQ(lastRemaining(4, 1), 4);

    return check::report();
}
