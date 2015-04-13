
#include "debug.hpp"
#include "pool.hpp"
#include "tap.hpp"

#include <set>
#include <vector>
#include <algorithm>

using namespace std;
using namespace util;

void
check_single (void) {
    // Ensure a single element doesn't break the circular linked list
    pool<uint64_t> single(1);
    single.release (single.acquire ());
}


void
check_unique_ptr (void) {
    pool<uint64_t> uintpool (1025);
    set<uint64_t *> uintset;

    // Take all pointers out, checking they are unique, then replace for destruction.
    for (unsigned int i = 0; i < uintpool.capacity (); ++i) {
        bool success = uintset.insert (uintpool.acquire ()).second;
        CHECK (success);
    }

    for (auto i = uintset.begin (); i != uintset.end (); ++i) 
        uintpool.release (*i);
    uintset.clear ();

    // Do the above one more time to ensure that releasing works right
    for (unsigned int i = 0; i < uintpool.capacity (); ++i) {
        bool success = uintset.insert (uintpool.acquire ()).second;
        CHECK (success);
    }

    for (auto i = uintset.begin (); i != uintset.end (); ++i) 
        uintpool.release (*i);
}


void
check_keep_value (void) {
    // Ensure that items keep their values.
    pool<uint64_t> uintpool(256);
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
    vector<bool> present(uintpool.capacity (), false);
    for (auto i = uintvector.begin (); i != uintvector.end (); ++i) {
        CHECK (**i < uintpool.capacity ());
        CHECK (present[**i] != true);

        present[**i] = true;
    }

    // All must have been marked as present...
    CHECK (find (present.begin (), present.end (), false) == present.end ());

    // Release all back into the pool for destruction
    for (auto i = uintvector.begin (); i != uintvector.end (); ++i)
        uintpool.release (*i);
    uintvector.clear ();
}


int 
main (int, char **) {
    check_single ();
    check_unique_ptr ();
    check_keep_value ();

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}
