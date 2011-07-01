
#include "debug.hpp"
#include "pool.hpp"

#include <set>
#include <vector>
#include <algorithm>

using namespace std;


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
        check_hard (success);
    }

    for (auto i = uintset.begin (); i != uintset.end (); ++i) 
        uintpool.release (*i);
    uintset.clear ();

    // Do the above one more time to ensure that releasing works right
    for (unsigned int i = 0; i < uintpool.capacity (); ++i) {
        bool success = uintset.insert (uintpool.acquire ()).second;
        check_hard (success);
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
        uint64_t *uint = uintpool.acquire ();
        *uint = i;

        uintvector.push_back(uint);
    }
    check (uintvector.size () == uintpool.capacity ());

    // Ensure they're all still present
    vector<bool> present(uintpool.capacity (), false);
    for (auto i = uintvector.begin (); i != uintvector.end (); ++i) {
        check_hard (**i < uintpool.capacity ());
        check_hard (present[**i] != true);

        present[**i] = true;
    }

    // All must have been marked as present...
    check_hard (find (present.begin (), present.end (), false) == present.end ());

    // Release all back into the pool for destruction
    for (auto i = uintvector.begin (); i != uintvector.end (); ++i)
        uintpool.release (*i);
    uintvector.clear ();
}


int 
main (int argc, char **argv) {
    check_single ();
    check_unique_ptr ();
    check_keep_value ();
}
