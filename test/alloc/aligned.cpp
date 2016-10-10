#include "tap.hpp"

#include "alloc/aligned.hpp"
#include "alloc/linear.hpp"


int
main (int, char**)
{
    util::TAP::logger tap;

    // set aside a sizeable buffer. it has to be large enough to conceivably
    // satisfy a sane allocation request during testing, just in case the
    // underlying code actually decides to do something; we don't be touching
    // it ourselves.
    static char buffer[1024*1024];

    // pick an alignment that isn't likely to be satisfied by any likely
    // underlying allocator. if the allocation fulfills this alignment then
    // we're probably operating correctly. 
    static constexpr std::size_t alignment = 3;

    util::alloc::aligned<util::alloc::linear> alloc (
        alignment, std::begin (buffer), std::end (buffer)
    );

    // allocate a range of values and make sure they all satisfy our alignment.
    // don't choose values which are likely to combine with the testing
    // alignment to produce a likely system alignment. eg, 3 + 5 == 8 which is
    // a power-of-2.
    uintptr_t result[4] = {
        (uintptr_t)alloc.allocate (9),     // just over a power of two
        (uintptr_t)alloc.allocate (1),     // a single byte
        (uintptr_t)alloc.allocate (64),    // a cache line
        (uintptr_t)alloc.allocate (250)    // multiple cache lines, but not a power of two
    };

    tap.expect (
        !(result[0] % alignment) &&
        !(result[1] % alignment) &&
        !(result[2] % alignment) &&
        !(result[3] % alignment),

        "allocations make alignment of %zu", alignment
    );

    return tap.status ();
}
