#include "../../tap.hpp"

#include "../../encode/base.hpp"
#include "../../iterator.hpp"


///////////////////////////////////////////////////////////////////////////////
static constexpr char input[] = "foobar";


//-----------------------------------------------------------------------------
// test vectors from rfc4648
template <int Size>
struct output { static const char *value[std::size (input)]; };

//-----------------------------------------------------------------------------
template <>
const char* output<64>::value[] = {
    "", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy",
};


template <>
const char* output<32>::value[]  {
    "", "MY======", "MZXQ====", "MZXW6===", "MZXW6YQ=", "MZXW6YTB", "MZXW6YTBOI======"
};


#if 0
template <>
const char* output<32h>::value[] {
    "", "CO======", "CPNG====", "CPNMU===", "CPNMUOG=", "CPNMUOJ1", "CPNMUOJ1E8======",
}
#endif


//-----------------------------------------------------------------------------
template <>
const char *output<16>::value[] = {
    "", "66", "666F", "666F6F", "666F6F62", "666F6F6261", "666F6F626172"
};


//-----------------------------------------------------------------------------
template <int Size>
const auto output_v = output<Size>::value;


///////////////////////////////////////////////////////////////////////////////
template <int Size>
void
test_size (util::TAP::logger &tap)
{
    for (unsigned i = 0; i < std::size (input); ++i) {
        const auto fragment = util::make_view (input, input + i);

        std::string encoded;

        util::encode::base<Size>::encode (
            std::back_inserter (encoded),
            fragment
        );

        tap.expect_eq (encoded, output_v<Size>[i],
            "%! character base%! encode; '%!' is '%!'", i, Size, encoded, output_v<Size>[i]
        );


        std::string decoded;
        util::encode::base<Size>::decode (
            std::back_inserter (decoded),
            util::make_view (encoded)
        );

        tap.expect_eq (decoded, fragment, "%! character base%! decode, '%!'", i, Size, decoded);
    }
}


///////////////////////////////////////////////////////////////////////////////
int
main ()
{
    util::TAP::logger tap;

    test_size<16> (tap);
    test_size<32> (tap);
    test_size<64> (tap);

    return tap.status ();
}