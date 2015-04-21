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
    }

    {
        using tuple_t = std::tuple<float>;
        using mapped_t = typename util::tuple::map<tuple_t, int_mapper>::type;

        bool tuple  = std::is_same<typename std::tuple_element<0, tuple_t >::type, int>::value;
        bool mapped = std::is_same<typename std::tuple_element<0, mapped_t>::type, int>::value;

        tap.expect (!tuple && mapped, "tuple type mapping");
    }
}
