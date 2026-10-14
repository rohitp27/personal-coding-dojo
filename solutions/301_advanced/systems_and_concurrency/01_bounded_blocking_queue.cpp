// 301 advanced > systems_and_concurrency > 01 Bounded blocking queue (canonical form)
//
// Problem:
//   Implement BoundedQueue<T>, a FIFO queue with a fixed capacity that is safe
//   for many producer and many consumer threads, using std::mutex and
//   std::condition_variable (no busy-waiting, no sleeping loops).
//
//     push(v)           block while the queue is full; enqueue and return true.
//                       Return false (v NOT enqueued) if the queue is, or becomes, closed.
//     tryPush(v)        never blocks; false if full or closed.
//     pop(out)          block while empty and open. Returns true with an item, or
//                       false only when the queue is closed AND drained.
//     tryPop(out)       never blocks; false if empty.
//     popFor(out, d)    like pop but gives up after duration d (false on timeout
//                       or closed-and-drained).
//     close()           no more pushes are accepted; wakes EVERY blocked thread.
//                       Items already queued can still be popped. Idempotent.
//     closed(), size()  size() never exceeds the capacity.
//
// Examples (capacity 2):
//   push 1, push 2, tryPush 3 -> false;  pop -> 1; pop -> 2  (FIFO)
//   one consumer blocked in pop on an empty queue, then close() -> pop returns false
//   queue holds {1}, then close(): pop -> true (1), next pop -> false
//
// Constraints: capacity >= 1; T is movable. Any number of threads.
//
// Required complexity: O(1) per operation (amortized), no lost wake-ups, no spurious
//   wake-up bugs, never holds the lock while waiting for anything else.
//
// Assumptions: close() is the only shutdown mechanism; destroying a queue that
//   still has blocked threads is the caller's bug.
//
// Think first:
//   1. What can go wrong? List at least: lost wake-up, spurious wake-up, waiting
//      with `if` instead of a predicate, waking only one thread on close, a
//      producer that stays blocked forever after close.
//   2. State which mutex protects which fields, and which condition each
//      condition_variable represents ("not full", "not empty") - and who notifies it.
//   3. notify_one vs notify_all: where is each correct, and what happens with
//      several consumers and one produced item?
//
// BEFORE CODING: write what can go wrong, which lock protects what, and your
// approach in a comment block right here.
//
// Run: make run FILE=curriculum/301_advanced/systems_and_concurrency/01_bounded_blocking_queue.cpp
// Concurrency tests have deterministic outcomes (joins, counters, bounded waits).
// A watchdog aborts with a message if your code deadlocks.

#include "check.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// ===== YOUR CODE BELOW =====
template <typename T>
class BoundedQueue {
    size_t cap_;
    std::deque<T> q_;
    mutable std::mutex m_;
    std::condition_variable notFull_, notEmpty_;
    bool closed_ = false;

public:
    explicit BoundedQueue(size_t capacity) : cap_(capacity) {}
    bool push(T v) {
        std::unique_lock<std::mutex> lk(m_);
        notFull_.wait(lk, [&] { return closed_ || q_.size() < cap_; });
        if (closed_) return false;
        q_.push_back(std::move(v));
        notEmpty_.notify_one();
        return true;
    }
    bool tryPush(T v) {
        std::lock_guard<std::mutex> lk(m_);
        if (closed_ || q_.size() >= cap_) return false;
        q_.push_back(std::move(v));
        notEmpty_.notify_one();
        return true;
    }
    bool pop(T& out) {
        std::unique_lock<std::mutex> lk(m_);
        notEmpty_.wait(lk, [&] { return closed_ || !q_.empty(); });
        if (q_.empty()) return false;
        out = std::move(q_.front());
        q_.pop_front();
        notFull_.notify_one();
        return true;
    }
    bool tryPop(T& out) {
        std::lock_guard<std::mutex> lk(m_);
        if (q_.empty()) return false;
        out = std::move(q_.front());
        q_.pop_front();
        notFull_.notify_one();
        return true;
    }
    bool popFor(T& out, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lk(m_);
        if (!notEmpty_.wait_for(lk, timeout, [&] { return closed_ || !q_.empty(); })) return false;
        if (q_.empty()) return false;
        out = std::move(q_.front());
        q_.pop_front();
        notFull_.notify_one();
        return true;
    }
    void close() {
        {
            std::lock_guard<std::mutex> lk(m_);
            closed_ = true;
        }
        notFull_.notify_all();
        notEmpty_.notify_all();
    }
    bool closed() const {
        std::lock_guard<std::mutex> lk(m_);
        return closed_;
    }
    size_t size() const {
        std::lock_guard<std::mutex> lk(m_);
        return q_.size();
    }
};
// ===== TESTS (do not edit) =====
using namespace std::chrono_literals;

static void startWatchdog(int secs) {
    std::thread([secs] {
        std::this_thread::sleep_for(std::chrono::seconds(secs));
        std::cerr << "WATCHDOG: tests ran longer than " << secs << "s - deadlock or lost wake-up?\n";
        std::_Exit(3);
    }).detach();
}

static void awaitTrue(std::atomic<bool>& flag, const char* what) {
    auto deadline = std::chrono::steady_clock::now() + 5s;
    while (!flag && std::chrono::steady_clock::now() < deadline) std::this_thread::sleep_for(1ms);
    if (!flag) {
        std::cerr << "TIMEOUT waiting for: " << what << " (blocked forever?)\n";
        std::_Exit(2);
    }
}

int main() {
    startWatchdog(40);

    // single-threaded semantics
    {
        BoundedQueue<int> q(2);
        int x = 0;
        CHECK(!q.closed());
        CHECK(!q.tryPop(x));
        CHECK(q.tryPush(1));
        CHECK(q.tryPush(2));
        CHECK(!q.tryPush(3));
        CHECK_EQ(q.size(), (size_t)2);
        CHECK(q.pop(x));
        CHECK_EQ(x, 1);
        CHECK(q.push(3));  // space again, so push does not block
        CHECK(q.pop(x));
        CHECK_EQ(x, 2);
        CHECK(q.tryPop(x));
        CHECK_EQ(x, 3);
        CHECK_EQ(q.size(), (size_t)0);
    }

    // a producer blocks on a full queue until a consumer makes room
    {
        BoundedQueue<int> q(1);
        CHECK(q.push(1));
        std::atomic<bool> started{false}, done{false};
        bool pushed = false;
        std::thread t([&] {
            started = true;
            pushed = q.push(2);
            done = true;
        });
        awaitTrue(started, "producer thread start");
        std::this_thread::sleep_for(50ms);
        CHECK(!done);  // a correct push cannot have completed: the queue is full
        int x = 0;
        CHECK(q.pop(x));
        CHECK_EQ(x, 1);
        awaitTrue(done, "blocked push to finish after a pop");
        t.join();
        CHECK(pushed);
        CHECK(q.pop(x));
        CHECK_EQ(x, 2);
    }

    // a consumer blocks on an empty queue until an item arrives
    {
        BoundedQueue<int> q(4);
        std::atomic<bool> started{false}, done{false};
        int got = -1;
        bool ok = false;
        std::thread t([&] {
            started = true;
            ok = q.pop(got);
            done = true;
        });
        awaitTrue(started, "consumer thread start");
        std::this_thread::sleep_for(50ms);
        CHECK(!done);
        CHECK(q.push(42));
        awaitTrue(done, "blocked pop to finish after a push");
        t.join();
        CHECK(ok);
        CHECK_EQ(got, 42);
    }

    // close() wakes every blocked consumer
    {
        BoundedQueue<int> q(4);
        const int N = 3;
        std::atomic<int> finished{0};
        std::vector<char> result(N, 1);
        std::vector<std::thread> ts;
        for (int i = 0; i < N; i++)
            ts.emplace_back([&, i] {
                int x;
                result[i] = q.pop(x);
                finished++;
            });
        std::this_thread::sleep_for(50ms);
        CHECK_EQ(finished.load(), 0);
        q.close();
        q.close();  // idempotent
        for (auto& t : ts) t.join();
        CHECK_EQ(finished.load(), N);
        CHECK_EQ((int)std::count(result.begin(), result.end(), (char)0), N);
        CHECK(q.closed());
    }

    // close() wakes a blocked producer; queued items stay poppable; later pushes fail
    {
        BoundedQueue<int> q(1);
        CHECK(q.push(1));
        std::atomic<bool> started{false}, done{false};
        bool pushed = true;
        std::thread t([&] {
            started = true;
            pushed = q.push(2);
            done = true;
        });
        awaitTrue(started, "producer thread start");
        std::this_thread::sleep_for(50ms);
        CHECK(!done);
        q.close();
        awaitTrue(done, "blocked producer to wake on close");
        t.join();
        CHECK(!pushed);
        CHECK(!q.push(9));
        CHECK(!q.tryPush(9));
        int x = 0;
        CHECK(q.pop(x));  // the item that was queued before close
        CHECK_EQ(x, 1);
        CHECK(!q.pop(x));  // drained and closed
        CHECK(!q.tryPop(x));
    }

    // popFor: timeout, success and closed
    {
        BoundedQueue<int> q(2);
        int x = 0;
        auto t0 = std::chrono::steady_clock::now();
        CHECK(!q.popFor(x, 30ms));
        auto waited = std::chrono::steady_clock::now() - t0;
        CHECK(waited >= 25ms);  // must actually wait (wait_for with a predicate)

        std::thread producer([&] {
            std::this_thread::sleep_for(30ms);
            q.push(9);
        });
        CHECK(q.popFor(x, 5000ms));
        CHECK_EQ(x, 9);
        producer.join();

        CHECK(q.push(5));
        CHECK(q.popFor(x, 0ms));  // item present: immediate
        CHECK_EQ(x, 5);

        q.close();
        t0 = std::chrono::steady_clock::now();
        CHECK(!q.popFor(x, 5000ms));  // closed and drained: returns promptly, not after 5s
        CHECK(std::chrono::steady_clock::now() - t0 < 2000ms);
    }

    // many producers, many consumers: nothing lost, nothing duplicated, per-producer FIFO,
    // and the size never exceeds the capacity
    {
        const size_t CAP = 4;
        const int P = 4, C = 4, PER = 5000;
        BoundedQueue<int> q(CAP);
        std::vector<std::vector<int>> got(C);
        std::vector<std::thread> consumers, producers;
        std::atomic<bool> stopMonitor{false};
        std::atomic<size_t> maxSize{0};
        std::thread monitor([&] {
            while (!stopMonitor) {
                size_t s = q.size();
                size_t prev = maxSize.load();
                while (s > prev && !maxSize.compare_exchange_weak(prev, s)) {}
                std::this_thread::sleep_for(50us);
            }
        });
        for (int c = 0; c < C; c++)
            consumers.emplace_back([&, c] {
                int x;
                while (q.pop(x)) got[c].push_back(x);
            });
        for (int p = 0; p < P; p++)
            producers.emplace_back([&, p] {
                for (int i = 0; i < PER; i++) q.push(p * PER + i);
            });
        for (auto& t : producers) t.join();
        q.close();
        for (auto& t : consumers) t.join();
        stopMonitor = true;
        monitor.join();

        std::vector<int> all;
        for (auto& g : got) all.insert(all.end(), g.begin(), g.end());
        CHECK_EQ(all.size(), (size_t)(P * PER));
        std::sort(all.begin(), all.end());
        std::vector<int> expected(P * PER);
        for (int i = 0; i < P * PER; i++) expected[i] = i;
        CHECK(all == expected);

        bool fifo = true;
        for (auto& g : got) {
            std::vector<int> last(P, -1);
            for (int v : g) {
                int p = v / PER;
                if (v <= last[p]) fifo = false;
                last[p] = v;
            }
        }
        CHECK(fifo);
        CHECK(maxSize.load() <= CAP);
    }

    return check::report();
}
