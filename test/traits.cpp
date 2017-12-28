#include <cruft/util/tap.hpp>
#include <cruft/util/types/traits.hpp>

#include <list>


///////////////////////////////////////////////////////////////////////////////
template <typename FuncT>
constexpr bool
returns_int (FuncT)
{
    return std::is_same_v<
        int,
        typename func_traits<FuncT>::return_type
    >;
}


///////////////////////////////////////////////////////////////////////////////
template <typename, class = std::void_t<>>
struct has_return_type : std::false_type {};


//-----------------------------------------------------------------------------
template <typename T>
struct has_return_type<
    T, std::void_t<typename func_traits<T>::return_type>
> : std::true_type {};


///////////////////////////////////////////////////////////////////////////////
extern int plain (char, float) { return 0; }
extern int with_noexcept (char, float) noexcept { return 0; }

struct foo {
    int bar (double, char) const { return 0; }
    int with_noexcept (double, char) noexcept { return 0; }
};


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    tap.expect (returns_int ( plain), "free-function by-reference");
    tap.expect (returns_int (&plain), "free-function by-pointer");

    tap.expect (returns_int ( with_noexcept), "free-function with noexcept by-reference");
    tap.expect (returns_int (&with_noexcept), "free-function with noexcept by-pointer");

    tap.expect (returns_int (&foo::bar), "member-function by reference");
    tap.expect (returns_int (&foo::with_noexcept), "member-function with noexcept by reference");

    using all_t = void (*const &)(void *, unsigned int, unsigned int, void **) noexcept;
    tap.expect (std::is_same_v<void, typename func_traits<all_t>::return_type>,
                "complex function type");

    tap.expect (!has_return_type<int>::value, "integer is not applicable to func_traits");


    tap.expect (is_contiguous_v<std::vector<char>>, "vector is contiguous");
    tap.expect (is_contiguous_v<std::array<char,4>>, "array is contiguous");
    tap.expect (!is_contiguous_v<std::list<char>>, "list is not contiguous");
    return tap.status ();
}