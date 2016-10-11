#include "crypto/ice.hpp"
#include "tap.hpp"
#include "iterator.hpp"
#include "stream.hpp"
#include "endian.hpp"

#include <iostream>
#include <iomanip>
#include <array>
#include <algorithm>
    
int
main (int, char**)
{
    const uint64_t data = 0xfedcba9876543210;

    struct {
        unsigned level;
        uint64_t crypt;
        std::vector<uint64_t> key;
    } TESTS[] = {
        { 0, 0xde240d83a00a9cc0, { 0xdeadbeef01234567 } },
        { 1, 0x7d6ef1ef30d47a96, { 0xdeadbeef01234567 } },
        { 2, 0xf94840d86972f21c, { 0x0011223344556677, 0x8899aabbccddeeff } },
    };

    util::TAP::logger tap;

    for (const auto &t: TESTS) {
        std::vector<uint64_t> k (t.key.cbegin (), t.key.cend ());
        std::transform (k.cbegin (), k.cend (), k.begin (), util::hton<uint64_t>);

        ice key (t.level, k.data (), k.data () + k.size ());

        auto crypt = key.encrypt (data);
        auto plain = key.decrypt (t.crypt);

        tap.expect_eq (crypt, t.crypt, "ICE level %u certification, encrypt", t.level);
        tap.expect_eq (plain, data, "ICE level %u certification, decrypt", t.level);
    }
    
    return tap.status ();
}
