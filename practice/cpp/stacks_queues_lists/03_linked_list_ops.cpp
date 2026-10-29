// Problem: Linked List Surgery - Reverse in K-Groups and Find the Cycle Start
// Two singly-linked-list functions. Both must relink existing nodes (never copy values
// between nodes) and use O(1) extra space.
//
// (a) reverseKGroup(head, k): reverse the list k nodes at a time and return the new head.
//     If fewer than k nodes remain at the end, leave that last group as it is. k >= 1.
//       {1,2,3,4,5}, k=2 -> {2,1,4,3,5}
//       {1,2,3,4,5}, k=3 -> {3,2,1,4,5}
//       {1,2,3,4,5}, k=1 -> unchanged;  k > length -> unchanged
//
// (b) cycleStart(head): if the list has a cycle (some node's next points back to an earlier
//     node), return the node where the cycle begins; otherwise return nullptr.
//       {3,2,0,-4} with the tail linked back to the node holding 2 -> the node holding 2
//
// Constraints: 0 <= length <= 1,000,000. Recursion that uses one stack frame per group
// or per node can crash on the large tests - the space requirement is O(1).
// Required complexity: O(n) time, O(1) extra space for both.
//
// Think first:
//   1. For (a), before reversing a group, how do you know there are at least k nodes left,
//      and which pointers must you remember so the reversed group can be reconnected to
//      the part before it and the part after it?
//   2. For (a), what does the returned head look like when k == 1, k == length, k > length,
//      and for an empty list?
//   3. For (b), after the slow and fast pointers meet inside the cycle, why does restarting
//      one pointer at the head and stepping both one node at a time make them meet exactly
//      at the cycle start?
//
// Run: make run FILE=practice/cpp/stacks_queues_lists/03_linked_list_ops.cpp

#include "check.h"

#include <algorithm>
#include <deque>
#include <numeric>
#include <vector>
#include <assert.h>

struct ListNode {
    int val;
    ListNode* next;
};

int listSize(ListNode* head) {
    int count = 0;
    ListNode* current = head;
    while(current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

ListNode* reverseList(ListNode* previous, ListNode* current) {   
    if (current == nullptr) return previous;

    ListNode* oldNext = current->next;
    current->next = previous;
    return reverseList(current, oldNext);
}

ListNode* reverseListWithCheck(ListNode* head, int k) {
    assert(listSize(head) == k);
    return reverseList(nullptr, head);
} 

ListNode* getLastNode(ListNode* head) {
    assert(head != nullptr);
    if (head -> next == nullptr) return head;
    return getLastNode(head->next);
}

ListNode* reverseKGroup(ListNode* head, int k, int remainingSegments) {
    if (remainingSegments == 0) {
        return head;
    }
    // find seg start/end whatever
    ListNode* segStart = head;
    ListNode* segEnd = nullptr;
    ListNode* segTail = segStart;

    int hops = 0;
    while (hops++ < k) { 
        segEnd = segTail;
        segTail = segTail->next;
    }
    // splice the tail off
    assert(segEnd->next == segTail);
    segEnd->next = nullptr;
    ListNode* reversed = reverseListWithCheck(segStart, k);
    assert(getLastNode(reversed) == segStart);
    assert(reversed == segEnd); // last node should be new head of 'reversed'
    // reattach at bottom
    ListNode* newTail = reverseKGroup(segTail, k, --remainingSegments);
    segStart->next = newTail;
    return segEnd;
}

ListNode* reverseKGroup(ListNode* head, int k) {
    int size = listSize(head);
    if (size < k) return head; // no flipping;
    if (k == 1) return head; // k=1 means every element flips in place

    int segments = size / k;
    return reverseKGroup(head, k, segments);
}

ListNode* moveSlowPtr(ListNode* slowptr) {
    if (slowptr == nullptr) {
        return nullptr;
    }
    return slowptr->next;
}

ListNode* moveFastPtr(ListNode* fastptr) {
    return moveSlowPtr(moveSlowPtr(fastptr));
}

ListNode* getCycleMeetup(ListNode* head) {
    ListNode* fastPtr = head;
    ListNode* slowPtr = head;

    while(true) { // some condition here
        // move slow ptr
        slowPtr = moveSlowPtr(slowPtr);
        fastPtr = moveFastPtr(fastPtr);
        if (slowPtr == nullptr || fastPtr == nullptr) {
            // reached end
            return nullptr; 
        }
        else if (slowPtr == fastPtr) {
            // only possible if there was a cycle
            return slowPtr;
        } else {
            // nothing here for now
        }
    }
}

// assume a cycle exists
ListNode* getCycleStart(ListNode* head, ListNode* meetupNode) {
    ListNode* ptrA = head;
    ListNode* ptrB = meetupNode;
    // printf("Checking start pt on : %d, %d \n", ptrA->val, ptrB->val);
    while (true)
    {
        ptrA = moveSlowPtr(ptrA);
        ptrB = moveSlowPtr(ptrB);
        assert(ptrA != nullptr);
        assert(ptrB != nullptr);
        if (ptrA == ptrB) {
            return ptrA;
        }
    }
}

ListNode* cycleStart( ListNode* head) {
    ListNode* cycleCheckMeetup = getCycleMeetup(head);
    if (cycleCheckMeetup == nullptr) {
        return nullptr;
    }
    if (cycleCheckMeetup == head) {
        return head;
    }
    // printf("Meet point at : %d \n", head->val);
    // run two ptrs from ot
    ListNode* cycleStartNode = getCycleStart(head, cycleCheckMeetup);
    // printf("Start point at : %d \n", head->val);
    return cycleStartNode;
}

// ---- test helpers (nodes live in a pool so the tests never leak) ----
static std::deque<ListNode> pool;

struct Built {
    ListNode* head;
    std::vector<ListNode*> nodes;
};

// Builds a list of the given values. If cyclePos >= 0, links the tail to nodes[cyclePos].
static Built build(const std::vector<int>& vals, int cyclePos = -1) {
    Built b{nullptr, {}};
    for (int v : vals) {
        pool.push_back({v, nullptr});
        b.nodes.push_back(&pool.back());
        if (b.nodes.size() > 1) b.nodes[b.nodes.size() - 2]->next = b.nodes.back();
    }
    if (!b.nodes.empty()) b.head = b.nodes[0];
    if (cyclePos >= 0 && !b.nodes.empty()) b.nodes.back()->next = b.nodes[cyclePos];
    return b;
}

// Reads a list; stops after a bound so a broken (cyclic) result cannot hang the test.
static std::vector<int> toVec(ListNode* h) {
    std::vector<int> out;
    for (size_t steps = 0; h && steps < 5000000; steps++, h = h->next) out.push_back(h->val);
    return out;
}

static std::vector<int> range(int n) {
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 1);
    return v;
}

static std::vector<int> expectedGroups(int n, int k) {
    std::vector<int> v = range(n);
    for (int i = 0; i + k <= n; i += k) std::reverse(v.begin() + i, v.begin() + i + k);
    return v;
}

int main() {
    using V = std::vector<int>;
    // --- (a) reverseKGroup ---
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 2)), (V{2, 1, 4, 3, 5}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 3)), (V{3, 2, 1, 4, 5}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5, 6}).head, 3)), (V{3, 2, 1, 6, 5, 4}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 1)), (V{1, 2, 3, 4, 5}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 5)), (V{5, 4, 3, 2, 1}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2, 3, 4, 5}).head, 6)), (V{1, 2, 3, 4, 5}));
    CHECK_EQ(toVec(reverseKGroup(build({1, 2}).head, 2)), (V{2, 1}));
    CHECK_EQ(toVec(reverseKGroup(build({1}).head, 1)), (V{1}));
    CHECK_EQ(toVec(reverseKGroup(build({1}).head, 2)), (V{1}));
    CHECK(reverseKGroup(nullptr, 3) == nullptr);
    {
        // nodes must be relinked, not copied: the new head is the old k-th node
        Built b = build({10, 20, 30, 40});
        ListNode* h = reverseKGroup(b.head, 2);
        CHECK(h == b.nodes[1]);
        CHECK(h->next == b.nodes[0]);
        CHECK(b.nodes[0]->next == b.nodes[3]);
        CHECK(b.nodes[3]->next == b.nodes[2]);
        CHECK(b.nodes[2]->next == nullptr);
        CHECK_EQ(b.nodes[1]->val, 20);  // values untouched
    }
    // large: 500,000 nodes. Deep recursion (one frame per group) overflows the stack for k=2.
    for (int k : {2, 3, 500000}) {
        Built b = build(range(500000));
        CHECK_EQ(toVec(reverseKGroup(b.head, k)) == expectedGroups(500000, k), true);
    }

    // --- (b) cycleStart ---
    CHECK(cycleStart(nullptr) == nullptr);
    CHECK(cycleStart(build({1}).head) == nullptr);
    CHECK(cycleStart(build({1, 2}).head) == nullptr);
    CHECK(cycleStart(build({1, 2, 3, 4, 5}).head) == nullptr);
    {
        Built b = build({1}, 0);  // single node pointing at itself
        CHECK(cycleStart(b.head) == b.nodes[0]);
    }
    {
        Built b = build({3, 2, 0, -4}, 1);
        CHECK(cycleStart(b.head) == b.nodes[1]);
    }
    {
        Built b = build({1, 2}, 0);
        CHECK(cycleStart(b.head) == b.nodes[0]);
    }
    {
        Built b = build({1, 2}, 1);  // tail points at itself
        CHECK(cycleStart(b.head) == b.nodes[1]);
    }
    {
        Built b = build({1, 2, 3, 4, 5, 6, 7}, 6);
        CHECK(cycleStart(b.head) == b.nodes[6]);
    }
    {
        Built b = build(range(1000000), 123456);
        CHECK(cycleStart(b.head) == b.nodes[123456]);
    }
    {
        Built b = build(range(1000000), 0);
        CHECK(cycleStart(b.head) == b.nodes[0]);
    }
    {
        Built b = build(range(1000000));
        CHECK(cycleStart(b.head) == nullptr);
    }

    return check::report();
}
