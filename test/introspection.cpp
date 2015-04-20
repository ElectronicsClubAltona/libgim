#include "introspection.hpp"
#include "tap.hpp"

//-----------------------------------------------------------------------------
// simple test struct of scalars
struct foo
{
    int a;
    float b;
};


//-----------------------------------------------------------------------------
// define introspection data
namespace util
{
    template <>
    struct type<foo>
    {
        typedef std::tuple<
            field<foo,int,&foo::a>,
            field<foo,float,&foo::b>
        > fields;
    };

    template <> const std::string field<foo,int,&foo::a>::name = "a";
    template <> const std::string field<foo,float,&foo::b>::name = "b";
}


//-----------------------------------------------------------------------------
int
main ()
{
    util::TAP::logger tap;
    
    foo d_foo { 7, 42.0 };
    auto f_tuple = util::as_tuple (d_foo);

    tap.expect (exactly_equal (d_foo.a, std::get<0> (f_tuple)) &&
                exactly_equal (d_foo.b, std::get<1> (f_tuple)),
                "member access after conversion to tuple");
}
