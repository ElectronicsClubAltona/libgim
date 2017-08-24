#include "tap.hpp"

#include "sarray.hpp"
#include "debug.hpp"

#include <vector>


///////////////////////////////////////////////////////////////////////////////
// A simple wrapper over an integer that increments at construction time, and
// decrements at destruction time. Used to check for obvious leakages in
// sarray lifetimes.
struct counter {
    explicit counter (int &_var):
        var (_var)
    { ++var; }

    counter (const counter &rhs):
        counter (rhs.var)
    { ; }

    counter& operator= (const counter &rhs)
    {
        var = rhs.var;
        return *this;
    }

    ~counter ()
    { --var; }

    int &var;
};


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**)
{
    util::TAP::logger tap;

    int count = 0;

    std::vector<counter> objects;
    std::generate_n (std::back_inserter (objects), 3, [&count] () { return counter (count); });
    CHECK_GT (objects.size (), 0u);

    {
        util::sarray<8,counter> array (objects);
        tap.expect_eq (
            count,
            static_cast<int> (objects.size () + array.size ()),
            "array constructors executed"
        );

        tap.expect_eq (
            objects.size (),
            array.size (),
            "array construction preserves size"
        );
    }

    tap.expect_eq (
        static_cast<int> (objects.size ()),
        count,
        "array destructors executed"
    );

    tap.expect_throw<std::length_error> ([] () {
        float data[] = { 0.f, 1.f, 2.f };
        util::sarray<2, float> value { data };
    }, "oversized array initializer throws std::length_error");

    return tap.status ();
}