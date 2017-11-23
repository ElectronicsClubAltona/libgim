#include "job/queue.hpp"
#include "tap.hpp"

#include <unistd.h>

int
main (void)
{
    util::TAP::logger tap;

    // dispatch `INNER' simple jobs `OUTTER' times that simply increment an
    // atomic variable and quit. this tests that all threads are created,
    // executed, and finished. it's not definitive, but executing this many
    // items this many times seems reasonably reliable in exposing deadlocks.
    bool success = true;
    constexpr int OUTTER = 16;
    constexpr int INNER  = 1024;

    for (auto i = 0; i < OUTTER && success; ++i) {
        std::atomic<int> count = 0;

        {
            util::job::queue q {};
            for (int j = 0; j < INNER; ++j)
                q.submit ([&count] () noexcept { ++count; });
            q.flush ();
        }

        success = count == INNER && success;
    }

    tap.expect (success, "trivial increment jobs");
    return tap.status ();
}