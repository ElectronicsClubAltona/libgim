#include "tuple.hpp"

#include "tap.hpp"

#include <typeindex>

template <typename T>
struct int_mapper
{
    typedef int type;
};

int
main ()
{
    util::TAP::logger tap;

    {
        auto tuple = std::make_tuple (1,2,3,4);
        std::vector<int> expected {{ 1, 2, 3, 4 }};

        std::vector<int> actual;
        util::tuple::for_each ([&actual] (auto i) { actual.push_back (i); }, tuple);

        tap.expect_eq (actual, expected, "value iteration");
    }

    {
    #if !defined(NO_RTTI)
        auto tuple = std::make_tuple (1u, 1, 1.f, 1.);
        std::vector<std::type_index> expected {
            std::type_index (typeid (1u)),
            std::type_index (typeid (1)),
            std::type_index (typeid (1.f)),
            std::type_index (typeid (1.))
        };

        std::vector<std::type_index> actual;
        util::tuple::for_type<decltype(tuple)> ([&actual] (auto i) {
            actual.push_back (typeid (typename decltype(i)::type));
        });

        tap.expect_eq (actual, expected, "type iteration");
    #else
        tap.skip ("type iteration");
    #endif
    }


    {
        using src_t = std::tuple<std::string>;
        using dst_t = typename util::tuple::map<src_t, int_mapper>::type;

        tap.expect (std::is_same<dst_t, std::tuple<int>>::value, "tuple type mapping");
    }
}
