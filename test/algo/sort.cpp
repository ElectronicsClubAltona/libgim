#include "algo/sort.hpp"
#include "tap.hpp"

#include <iterator>



void
reorder (std::vector<int> &values, std::vector<int> &indices)
{
    CHECK_EQ (values.size (), indices.size ());
    if (indices.size () <= 1)
        return;

    for (size_t i = 0; i < indices.size () - 1; ++i) {
        // check if this item is in the correct slot
        while ((int)i != indices[i]) {
            auto alt = indices[i];

            // swap this item with the one that's occupying its intended slot
            std::swap (values[i], values[alt]);
            //std::swap (*(values... + i), *)
            std::swap (indices[i], indices[alt]);
        }
    }
}


int
main (int, char**)
{
    util::TAP::logger tap;

    // Check that reorder works with a single value array
    {
        std::array<int,3> indices { 2, 0, 1 };
        std::array<int,3> values  { 3, 4, 5 };
        const std::array<int,3> expected = { 4, 5, 3 };

        cruft::util::sort::reorder (
            std::begin (indices), std::end (indices),
            std::begin (values)
        );

        tap.expect_eq (expected, values, "reorder with one array");
    }

    // Check that reorder works with multiple value arrays
    {
        std::array<int,3> indices { 2, 0, 1 };
        std::array<int,3> values[] = {
            {  3,  4,  5 },
            {  8,  7,  6 },
            { 11,  9, 10 },
        };
        const std::array<int,3> expected[] = {
            {  4,  5,  3 },
            {  7,  6,  8 },
            {  9, 10, 11 },
        };

        cruft::util::sort::reorder (
            std::begin (indices), std::end (indices),
            std::begin (values[0]),
            std::begin (values[1]),
            std::begin (values[2])
        );

        tap.expect (values[0] == expected[0] &&
                    values[1] == expected[1] &&
                    values[2] == expected[2],
            "reorder with three arrays"
        );
    }

    // Check that SOA sort works with a single value array
    static struct {
        std::vector<int> values;
        const char *msg;
    } TESTS[] = {
        { { 0 }, "single value, single array" },
        { { 1, 0 }, "two values, single array" },
        { { 5, 0, 4, 1, 2, 3 }, "many values, single array" },
    };

    for (auto &t: TESTS) {
        cruft::util::sort::soa (
            std::begin (t.values), std::end (t.values),
            [] (int a, int b) { return a < b; }
        );

        tap.expect (std::is_sorted (std::cbegin (t.values), std::cend (t.values)), "%s", t.msg);
    }

    // Check that SOA sort works with multiple value arrays
    {
        std::array<unsigned, 3> value_i { 2, 0, 1 };
        std::array<char, 3> value_c { 'a', 'b', 'c' };
        std::array<float,3> value_f { 0.f, 1.f, 2.f };

        const std::array<unsigned,3> expected_i { 0, 1, 2 };
        const std::array<char,3> expected_c { 'b', 'c', 'a' };
        const std::array<float,3> expected_f { 1.f, 2.f, 0.f };

        cruft::util::sort::soa (
            std::begin (value_i), std::end (value_i),
            [] (const auto &i, const auto &j) { return i < j; },
            std::begin (value_c),
            std::begin (value_f)
        );

        tap.expect (
            value_i == expected_i &&
            value_c == expected_c &&
            value_f == expected_f,
            "SOA sort with 3 arrays"
        );
    }

    return tap.status ();
}