#include "../tap.hpp"

#include "../iterator.hpp"

#include <vector>
#include <array>


///////////////////////////////////////////////////////////////////////////////
int
main (int, char**)
{
    util::TAP::logger tap;

    std::vector<int> v_int { 1, 2, 3 };
    std::array<float,3> a_float { 1.1f, 2.2f, 3.3f };

    bool success = true;
    for (auto [i, v, a]: util::izip (v_int, a_float)) {
        success = success && v_int[i] == v && util::exactly_equal (a_float[i], a);
    }

    tap.expect (success, "izip tuples of int and float");
    return tap.status ();
}
