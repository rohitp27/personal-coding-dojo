// 301 advanced > systems_and_concurrency > 03 Token-bucket rate limiter with an injected clock
//
// Problem:
//   Implement TokenBucket, a thread-safe rate limiter. It holds up to `capacity`
//   tokens and refills continuously at `tokensPerSecond` (fractional tokens
//   accumulate; nothing is rounded away between calls). It starts FULL.
//   Time comes ONLY from the injected clock `now()` (milliseconds, monotonic by
//   contract but the implementation must survive it going backwards) - never
//   read the real clock, so that behavior is fully deterministic.
//
//     tryAcquire(n=1)   all-or-nothing: if at least n tokens are available take n
//                       and return true; otherwise take nothing and return false.
//                       n > capacity can never succeed.
//     waitTimeMs(n=1)   milliseconds until n tokens would be available if nobody
//                       else acquires: 0 if available now, rounded UP to a whole
//                       millisecond, and -1 if it can never happen (n > capacity,
//                       or the rate is 0 and the tokens are not already there).
//                       Does not consume tokens.
//
//   If the clock moves backwards, ignore the jump: add no tokens, and do NOT
//   move your "last refill" point backwards (that would re-credit the same
//   interval when time moves forward again).
//
// Examples (capacity 5, 8 tokens/second, starts full):
//   five tryAcquire() -> true, sixth -> false
//   clock +125 ms -> exactly one token has refilled: tryAcquire() -> true, again -> false
//   after draining, waitTimeMs(1) = 125, waitTimeMs(2) = 250, waitTimeMs(6) = -1
//
// Constraints: capacity >= 1; rate >= 0 (rate 0 means no refill); many threads may
//   call concurrently.
//
// Required complexity: O(1) per call, no allocation, no sleeping inside the limiter.
//
// Assumptions: the clock is cheap and thread-safe to call; it is only called while
//   your lock is held (this is how the tests are written - no concurrent clock changes).
//   Tests only use "binary-friendly" rates (1, 2, 4, 8 per second) at exact boundaries,
//   so ordinary double arithmetic is fine.
//
// Think first:
//   1. What can go wrong? Races on (tokens, lastRefill) between two threads,
//      losing fractional refill by truncating elapsed time to whole tokens,
//      refilling past capacity, going negative or double-crediting on a
//      backwards clock.
//   2. Which single lock protects which fields? Why must "refill then decide"
//      be one critical section?
//   3. Lazy refill (compute on demand) vs a timer thread: why is lazy the only
//      design that works with an injected clock?
//
// BEFORE CODING: write what can go wrong, which lock protects what, and your
// approach in a comment block right here.
//
// Run: make run FILE=curriculum/301_advanced/systems_and_concurrency/03_token_bucket.cpp

#include "check.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// ===== YOUR CODE BELOW =====
#pragma GCC diagnostic ignored "-Wunused-parameter"  // stub only; delete when you implement

using NowMs = std::function<long long()>;

class TokenBucket {
public:
    TokenBucket(double tokensPerSecond, int capacity, NowMs now) {
        // TODO
    }
    bool tryAcquire(int n = 1) {
        // TODO
        return false;
    }
    long long waitTimeMs(int n = 1) {
        // TODO
        return -2;
    }
};

// ===== TESTS (do not edit) =====
static void startWatchdog(int secs) {
    std::thread([secs] {
        std::this_thread::sleep_for(std::chrono::seconds(secs));
        std::cerr << "WATCHDOG: tests ran longer than " << secs << "s - deadlock?\n";
        std::_Exit(3);
    }).detach();
}

int main() {
    startWatchdog(40);
    long long now = 0;
    NowMs clk = [&] { return now; };

    // starts full; five tokens then empty
    {
        now = 0;
        TokenBucket b(8.0, 5, clk);
        for (int i = 0; i < 5; i++) CHECK(b.tryAcquire());
        CHECK(!b.tryAcquire());
        now = 124;  // 0.992 of a token
        CHECK(!b.tryAcquire());
        now = 250;  // enough by now, even though the call at 124 saw a fraction
        CHECK(b.tryAcquire());
    }

    // exact boundary: 125 ms at 8/s is exactly one token
    {
        now = 0;
        TokenBucket b(8.0, 5, clk);
        for (int i = 0; i < 5; i++) CHECK(b.tryAcquire());
        now = 125;
        CHECK(b.tryAcquire());
        CHECK(!b.tryAcquire());
    }

    // refill is capped at capacity
    {
        now = 0;
        TokenBucket b(8.0, 5, clk);
        for (int i = 0; i < 5; i++) CHECK(b.tryAcquire());
        now = 1000000000LL;
        CHECK(b.tryAcquire(5));
        CHECK(!b.tryAcquire());
    }

    // starting at a nonzero time is still full from the beginning
    {
        now = 5000000;
        TokenBucket b(2.0, 3, clk);
        CHECK(b.tryAcquire(3));
        CHECK(!b.tryAcquire());
    }

    // multi-token requests are all-or-nothing
    {
        now = 0;
        TokenBucket b(8.0, 5, clk);
        CHECK(b.tryAcquire(3));
        CHECK(!b.tryAcquire(3));  // only 2 left, and nothing may be consumed by the failure
        CHECK(b.tryAcquire(2));
        CHECK(!b.tryAcquire(1));
        TokenBucket c(8.0, 5, clk);
        CHECK(!c.tryAcquire(6));  // more than capacity: never
        CHECK(c.tryAcquire(5));   // and the failed attempt took nothing
    }

    // waitTimeMs
    {
        now = 0;
        TokenBucket b(8.0, 5, clk);
        CHECK_EQ(b.waitTimeMs(5), 0LL);  // full bucket
        CHECK(b.tryAcquire(5));
        CHECK_EQ(b.waitTimeMs(1), 125LL);
        CHECK_EQ(b.waitTimeMs(2), 250LL);
        CHECK_EQ(b.waitTimeMs(5), 625LL);
        CHECK_EQ(b.waitTimeMs(6), -1LL);
        CHECK_EQ(b.waitTimeMs(1), 125LL);  // querying does not consume or advance anything
        now = 125;
        CHECK_EQ(b.waitTimeMs(1), 0LL);
        CHECK_EQ(b.waitTimeMs(3), 250LL);
        CHECK(b.tryAcquire());
    }
    {
        now = 0;
        TokenBucket b(3.0, 2, clk);  // 1000/3 = 333.33.. -> round up
        CHECK(b.tryAcquire(2));
        CHECK_EQ(b.waitTimeMs(1), 334LL);
        now = 333;
        CHECK(!b.tryAcquire());
        now = 334;
        CHECK(b.tryAcquire());
    }
    {
        now = 0;
        TokenBucket b(0.0, 2, clk);  // never refills
        CHECK_EQ(b.waitTimeMs(2), 0LL);
        CHECK(b.tryAcquire(2));
        CHECK_EQ(b.waitTimeMs(1), -1LL);
        now = 1000000;
        CHECK(!b.tryAcquire());
    }

    // clock going backwards: no credit, and no double credit afterwards
    {
        now = 1000;
        TokenBucket b(8.0, 4, clk);
        CHECK(b.tryAcquire(4));
        now = 1125;
        CHECK(b.tryAcquire());  // one token refilled
        now = 1000;             // jump back
        CHECK(!b.tryAcquire());
        now = 1125;             // back at the old high-water mark: still nothing new
        CHECK(!b.tryAcquire());
        now = 1250;             // 125 ms after the last real refill point
        CHECK(b.tryAcquire());
    }

    // thread safety: frozen clock, no refill, so exactly `capacity` acquisitions can ever succeed
    {
        now = 0;
        TokenBucket b(0.0, 1000, clk);
        std::atomic<int> ok{0};
        std::vector<std::thread> ts;
        for (int t = 0; t < 8; t++)
            ts.emplace_back([&] {
                for (int i = 0; i < 500; i++)
                    if (b.tryAcquire()) ok++;
            });
        for (auto& t : ts) t.join();
        CHECK_EQ(ok.load(), 1000);
    }
    // multi-token requests under contention: capacity 1000, requests of 3 -> exactly 333 succeed
    {
        now = 0;
        TokenBucket b(0.0, 1000, clk);
        std::atomic<int> ok{0};
        std::vector<std::thread> ts;
        for (int t = 0; t < 8; t++)
            ts.emplace_back([&] {
                for (int i = 0; i < 100; i++)
                    if (b.tryAcquire(3)) ok++;
            });
        for (auto& t : ts) t.join();
        CHECK_EQ(ok.load(), 333);
        CHECK(b.tryAcquire(1));
        CHECK(!b.tryAcquire(1));
    }

    return check::report();
}
