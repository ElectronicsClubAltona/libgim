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
    char c_char[] = { 'a', 'b', 'c' };

    bool success = true;
    for (auto [i, v, a, c]: util::izip (v_int, a_float, c_char)) {
        success = success &&
                  v_int[i] == v &&
                  util::exactly_equal (a_float[i], a) &&
                  c_char[i] == c;
    }

    tap.expect (success, "izip containers of int, float, and char and an initialiser_list");
    return tap.status ();
}
