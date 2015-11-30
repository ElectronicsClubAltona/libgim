#include "tap.hpp"
#include "alloc/linear.hpp"


int
main (void)
{
    util::TAP::logger tap;

    constexpr size_t BUFFER_SIZE = 1024;

    alignas (std::max_align_t) char memory[BUFFER_SIZE];
    util::alloc::linear store (std::begin (memory), std::end (memory));

    tap.expect_eq (store.capacity (), BUFFER_SIZE, "bytes capacity matches");

    tap.expect_throw<std::bad_alloc> (
        [&] (void) { store.allocate (BUFFER_SIZE + 1, 1); },
        "excessive allocation throws bad_alloc"
    );

    tap.expect_nothrow (
        [&] (void) { store.allocate (BUFFER_SIZE); },
        "maximum allocation succeeds"
    );

    tap.expect_eq (store.used (), BUFFER_SIZE, "bytes used matches");
    tap.expect_eq (store.remain (), 0u, "bytes remain matches");

    tap.expect_throw<std::bad_alloc> (
        [&] (void) { store.allocate (1, 1); },
        "minimum allocation fails after exhaustion"
    );

    store.reset ();

    tap.expect_nothrow (
        [&] (void) { store.allocate (1, 1); },
        "minimum allocation succeeds after reset"
    );

    return tap.status ();
}
