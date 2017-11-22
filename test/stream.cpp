#include "../tap.hpp"

#include "../stream.hpp"


///////////////////////////////////////////////////////////////////////////////
template <typename T>
void
test_scoped (util::TAP::logger&);


//-----------------------------------------------------------------------------
template <>
void
test_scoped<util::stream::scoped::flags> (util::TAP::logger &tap)
{
    std::ostringstream os;

    {
        util::stream::scoped::flags f (os);
        os << std::hex;
    }

    os << 15;
    
    tap.expect_eq (os.str (), "15", "stream::scoped::flag reverts std::hex");
}


///////////////////////////////////////////////////////////////////////////////
int
main (int, char **)
{
    util::TAP::logger tap;

    test_scoped<util::stream::scoped::flags> (tap);

    return tap.status ();
}
