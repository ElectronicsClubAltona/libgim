#include "rand/xorshift.hpp"
#include "rand/lcg.hpp"
#include "rand/mwc64x.hpp"

#include "tap.hpp"
#include "maths.hpp"
#include "types/string.hpp"


///////////////////////////////////////////////////////////////////////////////
template <>
std::string
type_to_string<util::rand::xorshift<uint32_t>> (void) { return "xorshift<uint32_t>"; }

template <>
std::string
type_to_string<util::rand::xorshift<uint64_t>> (void) { return "xorshift<uint64_t>"; }

template <> std::string type_to_string<util::rand::lcg_t> (void) { return "lcg_t"; }

template <> std::string type_to_string<util::rand::mwc64x> (void) { return "mwc64x"; }


///////////////////////////////////////////////////////////////////////////////
/// check random outputs are roughly divisible between a number of fixed width
/// buckets.
///
/// this is not anything close to a strict statistical test. it is more aimed
/// at link testing and basic functionality verification within a small
/// resource footprint (ie, fast unit testing).
template <typename G, typename ...Args>
void
test_buckets (util::TAP::logger &tap, Args&& ...args)
{
    constexpr unsigned BUCKET_BITS  = 8u;
    constexpr size_t   BUCKET_COUNT = 1u << BUCKET_BITS;
    constexpr unsigned BUCKET_MASK  = BUCKET_COUNT - 1u;
    constexpr unsigned EXPECTED     = 1024u;
    constexpr unsigned ITERATIONS = BUCKET_COUNT * EXPECTED;

    unsigned buckets[BUCKET_COUNT] = {};
    G gen (std::forward<Args> (args)...);

    for (unsigned i = 0; i < ITERATIONS; ++i)
        ++buckets[gen () & BUCKET_MASK];

    tap.expect (
        std::find_if (std::cbegin (buckets),
                      std::cend   (buckets),
                      [] (auto v) { return v < EXPECTED * 7 / 8; }) == std::cend (buckets),
        "bucket counts for %s", type_to_string<G> ());
}


///////////////////////////////////////////////////////////////////////////////
int
main (int,char**)
{
    util::TAP::logger tap;

    test_buckets<util::rand::xorshift<uint32_t>> (tap, 0x1234u);
    test_buckets<util::rand::xorshift<uint64_t>> (tap, 0x1234u);
    test_buckets<util::rand::lcg_t> (tap, 0x1234u);
    test_buckets<util::rand::mwc64x> (tap, 0x1234u);

    return tap.status ();
}
