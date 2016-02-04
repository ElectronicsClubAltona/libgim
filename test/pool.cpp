#include "pool.hpp"

#include "tap.hpp"

#include <set>
#include <vector>
#include <algorithm>


//-----------------------------------------------------------------------------
void
check_single (util::TAP::logger &tap)
{
    // Ensure a single element doesn't break the circular linked list
    util::pool<uint64_t> single(1);
    tap.expect_nothrow ([&] {
        single.release (single.acquire ());
    }, "single element acquire-release");
}


//-----------------------------------------------------------------------------
void
check_unique_ptr (util::TAP::logger &tap)
{
    util::pool<uint64_t> uintpool (1025);
    std::set<uint64_t *> uintset;

    // Take all pointers out, checking they are unique, then replace for destruction.
    while (!uintpool.empty ())
        uintset.insert (uintpool.acquire ());

    tap.expect_eq (uintset.size (), uintpool.capacity (), "extracted maximum elements");

    for (auto i: uintset)
        uintpool.release (i);

    tap.expect_eq (uintpool.size (), 0u, "re-inserted maximum elements");
    uintset.clear ();

    // Do the above one more time to ensure that releasing works right
    while (!uintpool.empty ())
        uintset.insert (uintpool.acquire ());
    tap.expect_eq (uintset.size (), uintpool.capacity (), "re-extracted maximum elements");
}


//-----------------------------------------------------------------------------
void
check_keep_value (util::TAP::logger &tap)
{
    // Ensure that items keep their values.
    util::pool<uint64_t> uintpool(256);
    std::vector<uint64_t*> uintvector;
    uintvector.reserve(uintpool.capacity ());

    // Give every item a unique value
    for (unsigned int i = 0; i < uintpool.capacity (); ++i) {
        uint64_t *item = uintpool.acquire ();
        *item = i;

        uintvector.push_back(item);
    }

    CHECK_EQ (uintvector.size (), uintpool.capacity ());

    // Ensure they're all still present
    std::vector<bool> present(uintpool.capacity (), false);
    for (auto i = uintvector.begin (); i != uintvector.end (); ++i) {
        CHECK (**i < uintpool.capacity ());
        CHECK (present[**i] != true);

        present[**i] = true;
    }

    // All must have been marked as present...
    tap.expect (std::find (present.begin (), present.end (), false) == present.end (), "values retained");

    // Release all back into the pool for destruction
    //for (auto i = uintvector.begin (); i != uintvector.end (); ++i)
    //    uintpool.release (*i);
    //uintvector.clear ();
}


//-----------------------------------------------------------------------------
int
main (int, char **)
{
    util::TAP::logger tap;

    check_single (tap);
    check_unique_ptr (tap);
    check_keep_value (tap);

    return tap.status ();
}
