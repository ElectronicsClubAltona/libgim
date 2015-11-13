#include "tap.hpp"
#include "alloc/stack.hpp"


///////////////////////////////////////////////////////////////////////////////
void
n_allocations (util::alloc::stack &store,
               unsigned count,
               size_t bytes,
               size_t alignment = alignof (std::max_align_t))
{
    for (unsigned i = 0; i < count; ++i) {
        auto ptr = store.allocate (bytes, alignment);
        store.deallocate (ptr, bytes, alignment);
    }
}


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    constexpr size_t BUFFER_AVAILABLE = 32;
    constexpr size_t BUFFER_REQUEST   = BUFFER_AVAILABLE - alignof (std::max_align_t);
    constexpr size_t BUFFER_PAD  = 32;
    constexpr size_t BUFFER_SIZE = BUFFER_AVAILABLE + BUFFER_PAD;

    // alignment is kinda important, so make it a little easier and ensure
    // something suitable right off the bat.
    alignas (std::max_align_t) char memory[BUFFER_SIZE];
    std::fill (std::begin (memory), std::end (memory), 0);

    util::alloc::stack store (memory, memory + BUFFER_AVAILABLE);

    // larger than total allocations should throw
    tap.expect_throw<std::bad_alloc> (
        [&store] (void) { store.allocate (BUFFER_AVAILABLE + 1, 1); },
        "excessive allocation throws bad_alloc"
    );

    // try a large number of allocations so we exercise the frame handling and
    // alignment routines.
    tap.expect_nothrow (
        [&store] (void) { n_allocations (store, BUFFER_AVAILABLE, BUFFER_REQUEST); },
        "repeated allocation/deallocation"
    );

    // perform two near maximum allocations and check for exhaustion through
    // bad_alloc
    auto ptr = store.allocate (BUFFER_REQUEST);
    (void)ptr;

    tap.expect_throw<std::bad_alloc> (
        [&store] (void) { store.allocate (BUFFER_REQUEST); },
        "bad_alloc thrown on exhaustion"
    );
    
    // try many allocations again after resetting the allocator to zero usage
    store.reset ();
    tap.expect_nothrow (
        [&store] (void) { n_allocations (store, BUFFER_AVAILABLE, BUFFER_REQUEST); },
        "no bad_alloc after reset"
    );

    return tap.status ();
}
