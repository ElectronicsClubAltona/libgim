#include "format.hpp"

#include "tap.hpp"


///////////////////////////////////////////////////////////////////////////////
struct userobj { };

static std::ostream&
operator<< (std::ostream &os, const userobj&)
{
    return os << "userobj";
}


///////////////////////////////////////////////////////////////////////////////
int
main (void)
{
    util::TAP::logger tap;

    #define CHECK_RENDER(fmt,res,...) do {                      \
        auto val = util::format::render (fmt, ##__VA_ARGS__);   \
        tap.expect_eq (val, res, "render '%s'", fmt);           \
    } while (0)

    CHECK_RENDER ("foo", "foo");

    CHECK_RENDER ("%i", "1", 1 );
    CHECK_RENDER ("%3i", "  1", 1 );
    CHECK_RENDER ("%03i", "001", 1 );
    CHECK_RENDER ("%.i", "", 0);
    CHECK_RENDER ("% .i", " ", 0); // zero precision still requires a space

    CHECK_RENDER ("%hhi", "1", (signed char)1);
    CHECK_RENDER ("%hi", "1",  (signed short)1);
    CHECK_RENDER ("%li", "1",  (signed long)1);
    CHECK_RENDER ("%lli", "1", (signed long long)1);
    CHECK_RENDER ("%ji", "1",  (intmax_t)1);
    CHECK_RENDER ("%zi", "1",  (ssize_t)1);
    CHECK_RENDER ("%ti", "1",  (ptrdiff_t)1);

    CHECK_RENDER ("%u", "1", 1u);
    CHECK_RENDER ("%03u", "001", 1u);
    CHECK_RENDER ("% u", " 1", 1u);
    CHECK_RENDER ("% 3u", "  1", 1u);
    CHECK_RENDER ("% 03u", " 01", 1u);
    CHECK_RENDER ("%-3u", "1  ", 1u);
    CHECK_RENDER ("%64u", "                                                               1", 1u);

    CHECK_RENDER ("%hhu", "1", (unsigned char)1);
    CHECK_RENDER ("%hu", "1",  (unsigned short)1);
    CHECK_RENDER ("%lu", "1",  (unsigned long)1);
    CHECK_RENDER ("%llu", "1", (unsigned long long)1);
    CHECK_RENDER ("%ju", "1",  (uintmax_t)1);
    CHECK_RENDER ("%zu", "0",  (size_t)0);
    CHECK_RENDER ("%zu", "1",  (size_t)1);
    CHECK_RENDER ("%!", "1", 1u);

    CHECK_RENDER ("%o", "1", 01u);
    CHECK_RENDER ("%o", "13", 013u);
    CHECK_RENDER ("%o", "13", 013u);
    CHECK_RENDER ("%#o", "013", 013u);

    CHECK_RENDER ("%x", "1", 0x1u);
    CHECK_RENDER ("%x", "fe", 0xfeu);
    CHECK_RENDER ("%X", "FE", 0xFEu);
    CHECK_RENDER ("%#x", "0xfe", 0xfeu);
    CHECK_RENDER ("%#X", "0XFE", 0xFEu);

    CHECK_RENDER ("%e", "1.000000e+00", 1.);
    CHECK_RENDER ("%e", "1.200000e+00", 1.2);
    CHECK_RENDER ("%e", "1.234568e+00", 1.2345678);

    CHECK_RENDER ("%E", "1.234568E+00", 1.2345678);

    CHECK_RENDER ("%f", "1.000000", 1.);
    CHECK_RENDER ("%f", "1.200000", 1.2);
    CHECK_RENDER ("%f", "1.234560", 1.23456);
    CHECK_RENDER ("%f", "1.234567", 1.234567);
    CHECK_RENDER ("%f", "1.234568", 1.2345678);

    CHECK_RENDER ("%g", "1", 1.);
    CHECK_RENDER ("%g", "1.2", 1.2);
    CHECK_RENDER ("%g", "1.23457", 1.2345678);
    CHECK_RENDER ("%g", "0.000123457", 0.00012345678);
    CHECK_RENDER ("%g", "1.23457e-05", 0.000012345678);
    CHECK_RENDER ("%G", "1.23457E-05", 0.000012345678);

    CHECK_RENDER ("%+e", "+1.234568e+00", 1.2345678);
    CHECK_RENDER ("%+f", "+1.234568", 1.2345678);
    CHECK_RENDER ("%+g", "+1.23457",  1.2345678);
    CHECK_RENDER ("%+a", "+0x1.3c0ca2a5b1d5dp+0", 1.2345678);

    CHECK_RENDER ("%#.e", "1.e+00", 1.2345678);
    CHECK_RENDER ("%#.f", "1.", 1.2345678);
    CHECK_RENDER ("%#.g", "1.", 1.2345678);
    //CHECK_RENDER ("%#.a", "0x1.p+0", 1.2345678);

    CHECK_RENDER ("%a", "0x1.3c0ca2a5b1d5dp+0", 1.2345678);
    CHECK_RENDER ("%A", "0X1.3C0CA2A5B1D5DP+0", 1.2345678);

    CHECK_RENDER ("%e", "inf", std::numeric_limits<double>::infinity ());
    CHECK_RENDER ("%E", "INF", std::numeric_limits<double>::infinity ());
    CHECK_RENDER ("%f", "inf", std::numeric_limits<double>::infinity ());
    CHECK_RENDER ("%F", "INF", std::numeric_limits<double>::infinity ());
    CHECK_RENDER ("%g", "inf", std::numeric_limits<double>::infinity ());
    CHECK_RENDER ("%G", "INF", std::numeric_limits<double>::infinity ());
    CHECK_RENDER ("%a", "inf", std::numeric_limits<double>::infinity ());
    CHECK_RENDER ("%A", "INF", std::numeric_limits<double>::infinity ());

    CHECK_RENDER ("%e", "nan", std::numeric_limits<double>::quiet_NaN ());
    CHECK_RENDER ("%E", "NAN", std::numeric_limits<double>::quiet_NaN ());
    CHECK_RENDER ("%f", "nan", std::numeric_limits<double>::quiet_NaN ());
    CHECK_RENDER ("%F", "NAN", std::numeric_limits<double>::quiet_NaN ());
    CHECK_RENDER ("%g", "nan", std::numeric_limits<double>::quiet_NaN ());
    CHECK_RENDER ("%G", "NAN", std::numeric_limits<double>::quiet_NaN ());
    CHECK_RENDER ("%a", "nan", std::numeric_limits<double>::quiet_NaN ());
    CHECK_RENDER ("%A", "NAN", std::numeric_limits<double>::quiet_NaN ());

    CHECK_RENDER ("%.f", "1", 1.2345678);
    CHECK_RENDER ("%3.f", "  1", 1.2345678);
    CHECK_RENDER ("%3.2f", "1.23", 1.2345678);
    CHECK_RENDER ("%3.2f", "1234.57", 1234.5678);

    CHECK_RENDER ("%!", "1", 1.);

    CHECK_RENDER ("%c", "A", 'A');
    CHECK_RENDER ("%!", "A", 'A');

    CHECK_RENDER ("%s", "foo", "foo");
    CHECK_RENDER ("%s", "foo", std::string ("foo"));
    CHECK_RENDER ("%s", "foo", const_cast<char*> ("foo"));
    CHECK_RENDER ("%.s", "", "foo");
    CHECK_RENDER ("%.0s", "", "foo");
    CHECK_RENDER ("%.2s", "fo", "foo");
    CHECK_RENDER ("%.02s", "fo", "foo");
    CHECK_RENDER ("%.64s", "foo", "foo");
    CHECK_RENDER ("%3.1s", "  f", "foo");
    CHECK_RENDER ("%-3.1s", "f  ", "foo");
    CHECK_RENDER ("%!", "foo", "foo");

    CHECK_RENDER ("%!", "userobj", userobj {});

    CHECK_RENDER ("%p", "0x1234567", (void*)0x01234567);
    CHECK_RENDER ("%p", "0x1234567", (int*)0x01234567);
    CHECK_RENDER ("%p", "0x1234567", (char*)0x01234567);
    CHECK_RENDER ("%p", "(nil)", nullptr);
    CHECK_RENDER ("%p", "(nil)", NULL);
    CHECK_RENDER ("%!", "0x1234567", (void*)0x01234567);

    CHECK_RENDER ("%%", "%");
    CHECK_RENDER ("%10%", "%");
    CHECK_RENDER ("%.%", "%");
    CHECK_RENDER ("% 0-+#12.34%", "%"); // escaped conversions should largely ignore flags, width, and precision.

    // multiple components
    CHECK_RENDER ("%%%%", "%%");

    CHECK_RENDER (" %%", " %");
    CHECK_RENDER ("%% ", "% ");
    CHECK_RENDER ("%% %%", "% %");
    CHECK_RENDER (" %% %% ", " % % ");

    CHECK_RENDER ("%%%d%%", "%0%", 0);

    CHECK_RENDER ("%u %u", "1 2", 1u, 2u);

    CHECK_RENDER ("%#o %o", "010 10", 8u, 8u);
    CHECK_RENDER ("%#o %o %#o", "010 10 010", 8u, 8u, 8u);
    CHECK_RENDER ("%X%x%X", "FfF", 0xfu, 0xfu, 0xfu);

    tap.expect_eq (util::format::render ("%u\n", 1u), "1\n", "newline");

    #define CHECK_THROW(fmt,except,...) do {                \
        tap.expect_throw<util::format::except> ([&] {       \
            util::format::render (fmt, ##__VA_ARGS__);      \
        }, "exception '%s' for format '%s'", #except, fmt); \
    } while (0)

    CHECK_THROW("%",  syntax_error);
    CHECK_THROW("%_", syntax_error);
    CHECK_THROW("%_u", syntax_error);

    CHECK_THROW("%u", missing_error);
    CHECK_THROW("%!", missing_error);

    CHECK_THROW("%d", conversion_error, 1u);
    CHECK_THROW("%i", conversion_error, 1u);
    CHECK_THROW("%i", conversion_error, nullptr);

    CHECK_THROW("%hhi", length_error, (long long)1);
    //CHECK_THROW("%lli", length_error, (signed char)1);

    CHECK_THROW("%u", conversion_error, 1.);
    CHECK_THROW("%u", conversion_error, "foo");
    CHECK_THROW("%u", conversion_error, (void*)0);
    CHECK_THROW("%u", conversion_error, 1);
    CHECK_THROW("%u", conversion_error, nullptr);

    CHECK_THROW("%hhu", length_error, (unsigned long long)1);
    //CHECK_THROW("%llu", length_error, (unsigned char)1);

    CHECK_THROW("%f", conversion_error, 1u);
    CHECK_THROW("%f", conversion_error, "foo");
    CHECK_THROW("%f", conversion_error, nullptr);

    CHECK_THROW("%s", conversion_error, 1u);
    CHECK_THROW("%s", conversion_error, '_');
    CHECK_THROW("%s", conversion_error, nullptr);

    CHECK_THROW("%c", conversion_error, 1u);
    CHECK_THROW("%c", conversion_error, "foo");
}
