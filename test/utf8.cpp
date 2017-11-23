#include "tap.hpp"
#include "utf8.hpp"

#include <vector>
#include <array>


///////////////////////////////////////////////////////////////////////////////
static void
simple_valid (util::TAP::logger &tap)
{
    static constexpr struct {
        const char *data;
        size_t len;
        const char *message;
    } VALID[] = {
        { "",        0, "empty string" },
        { "a",       1, "single ANSI character" },
        { "abc",     3, "multiple ANSI characters" },
        { u8"κόσμε", 5, "greek kosme" },
    };

    static constexpr char fmt[] = "valid length, %s";

    for (const auto &t: VALID) {
        try {
            tap.expect_eq (
                t.len,
                util::utf8::decode (util::make_view (t.data)).size (),
                fmt,
                t.message
            );
        } catch (...) {
            tap.fail (fmt, t.message);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
static void
single_boundaries (util::TAP::logger &tap)
{
    static const struct {
        std::vector<uint8_t> data;
        uint32_t value;
        const char *direction;
    } TESTS[] {
        { { 0x00                   }, 0x00000000, "low length boundary" },
        { { 0xC2, 0x80             }, 0x00000080, "low length boundary" },
        { { 0xE0, 0xA0, 0x80       }, 0x00000800, "low length boundary" },
        { { 0xF0, 0x90, 0x80, 0x80 }, 0x00010000, "low length boundary" },

        { { 0x7F                   }, 0x0000007F, "high length boundary" },
        { { 0xDF, 0xBF             }, 0x000007FF, "high length boundary" },
        // this is an invalid codepoint so we're going to fail to parse this
        // whatever the case. disable it for the time being.
        //{ { 0xEF, 0xBF, 0xBF,      }, 0x0000FFFF, "high length boundary" },
        { { 0xF7, 0xBF, 0xBF, 0xBF }, 0x001FFFFF, "high length boundary" },

        { { 0xED, 0x9F, 0xBF       }, 0x0000D7FF, "other" },
        { { 0xEE, 0x80, 0x80       }, 0x0000E000, "other" },
        { { 0xEF, 0xBF, 0xBD       }, 0x0000FFFD, "other" },
        { { 0xF4, 0x8F, 0xBF, 0xBF }, 0x0010FFFF, "other" },
        { { 0xF4, 0x90, 0x80, 0x80 }, 0x00110000, "other" },
    };

    static constexpr char fmt[] = "single character (%s), %!-byte sequence";

    for (const auto &t: TESTS) {
        auto data = util::make_view (
            reinterpret_cast<const std::byte*> (&*t.data.cbegin ()),
            reinterpret_cast<const std::byte*> (&*t.data.cbegin ()) + t.data.size ()
        );

        try {
            auto codepoints = util::utf8::decode (data);

            tap.expect (
                codepoints.size () == 1 && codepoints[0] == t.value,
                fmt,
                t.direction,
                t.data.size ()
            );
        } catch (const util::utf8::malformed_error&) {
            tap.fail (fmt, t.direction, t.data.size ());
        }
    }
};



///////////////////////////////////////////////////////////////////////////////
static void
malformed (util::TAP::logger &tap)
{
    static const struct {
        std::vector<uint8_t> data;
        const char *message;
    } TESTS[] = {
        { { 0x80                   }, "first continuation" },
        { { 0xBF                   }, "last continuation" },
        { { 0x80, 0xBF             }, "continuation sequence" },
        { { 0x80, 0xBF, 0x80       }, "continuation sequence" },
        { { 0x80, 0xBF, 0x80, 0xBF }, "continuation sequence" },
    };

    static constexpr char fmt[] = "malformed %! byte sequence, %s";

    for (const auto &t: TESTS) {
        auto data = util::make_view (
            reinterpret_cast<const std::byte*> (&*t.data.cbegin ()),
            reinterpret_cast<const std::byte*> (&*t.data.cbegin ()) + t.data.size ()
        );

        tap.expect_throw<util::utf8::malformed_error> (
            [&data] () {
                util::utf8::decode (data);
            },
            fmt,
            data.size (),
            t.message
        );
    }

    // test every continuation byte by itself. we use a boolean flag that
    // should never reach the line where we toggle it to false due to the
    // expected exception.
    {
        bool success = true;
        for (uint8_t c = 0x80; c <= 0xbf; ++c) {
            try {
                const auto v = c;
                util::utf8::decode (util::view { &v, &v+1 });
                success = false;
                break;
            } catch (...) { ; }
        }

        tap.expect (success, "individual continuation bytes");
    }

    // every combination of first-byte-then-space sequences
    static const struct {
        uint8_t first;
        uint8_t last;
        int length;
    } LONELY[] = {
        { 0xc0, 0xdf, 2 },
        { 0xe0, 0xef, 3 },
        { 0xf0, 0xf7, 4 },
    };

    for (const auto &t: LONELY) {
        union {
            uint8_t bytes[4];
            char    str  [4];
        };

        bool success = true;

        for (auto i = t.first; i <= t.last; ++i) {
            std::fill (std::begin (str), std::end (str), ' ');
            bytes[0] = i;

            try {
                util::utf8::decode (util::make_cview (str));
                success = false;
            }
            catch (const util::utf8::malformed_error&)
            { ; }
            catch (...)
            { success = false; }
        }

        tap.expect (success, "lonely start characters, %! bytes", t.length);
    }


    static const std::vector<uint8_t> MISSING[] = {
        { 0xC0 },
        { 0xE0, 0x80 },
        { 0xF0, 0x80, 0x80 },
    };

    for (const auto &t: MISSING) {
        util::view<const char*> data {
            reinterpret_cast<const char*> (&t[0]),
            reinterpret_cast<const char*> (&t[0]) + t.size ()
        };

        tap.expect_throw<util::utf8::malformed_error> (
            [&data] () { util::utf8::decode (data); },
            "%! byte sequence missing the lastbyte",
            t.size ()
        );
    }


    static const std::vector<uint8_t> IMPOSSIBLE[] = {
        { 0xfe },
        { 0xff },
        { 0xfe, 0xfe, 0xff, 0xff }
    };

    for (const auto &t: IMPOSSIBLE) {
        util::view<const char*> data {
            reinterpret_cast<const char*> (&t[0]),
            reinterpret_cast<const char*> (&t[0]) + t.size ()
        };

        tap.expect_throw<util::utf8::malformed_error> (
            [&data] () { util::utf8::decode (data); },
            "impossible %! byte sequence",
            t.size ()
        );
    }
};


///////////////////////////////////////////////////////////////////////////////
void
overlong (util::TAP::logger &tap)
{
    static const struct {
        std::vector<uint8_t> data;
        const char *message;
    } TESTS[] = {
        { { 0xc0, 0xaf             }, "simple ANSI" },
        { { 0xe0, 0x80, 0xaf       }, "simple ANSI" },
        { { 0xf0, 0x80, 0x80, 0xaf }, "simple ANSI" },

        { { 0xc1, 0xbf             }, "maximum" },
        { { 0xe0, 0x9f, 0xbf       }, "maximum" },
        { { 0xf0, 0x8f, 0xbf, 0xbf }, "maximum" },

        { { 0xc0, 0x80             }, "null" },
        { { 0xe0, 0x80, 0x80       }, "null" },
        { { 0xf0, 0x80, 0x80, 0x80 }, "null" },
    };

    for (const auto &t: TESTS) {
        auto data = util::make_view (
            reinterpret_cast<const char*> (&t.data[0]),
            reinterpret_cast<const char*> (&t.data[0]) + t.data.size ()
        );

        tap.expect_throw<util::utf8::overlong_error> (
            [&] () {
                util::utf8::decode (data);
            },
            "overlong %! byte sequence, %s",
            t.data.size (),
            t.message
        );
    }
};


///////////////////////////////////////////////////////////////////////////////
void
illegal (util::TAP::logger &tap)
{
    static const std::array<uint8_t,3> SINGLE[] = {
        { 0xed, 0xa0, 0x80 }, // U+D800
        { 0xed, 0xad, 0xbf }, // U+DB7F
        { 0xed, 0xae, 0x80 }, // U+DB80
        { 0xed, 0xaf, 0xbf }, // U+DBFF
        { 0xed, 0xb0, 0x80 }, // U+DC00
        { 0xed, 0xbe, 0x80 }, // U+DF80
        { 0xed, 0xbf, 0xbf }, // U+DFFF
    };

    for (const auto &t: SINGLE)
        tap.expect_throw<util::utf8::error> (
            [&t] () { util::utf8::decode (util::make_view (t)); },
            "reject utf16 single surrogate"
        );


    static const std::array<uint8_t,6> DOUBLE[] = {
        { 0xed, 0xa0, 0x80, 0xed, 0xb0, 0x80 }, // U+D800 U+DC00
        { 0xed, 0xa0, 0x80, 0xed, 0xbf, 0xbf }, // U+D800 U+DFFF
        { 0xed, 0xad, 0xbf, 0xed, 0xb0, 0x80 }, // U+DB7F U+DC00
        { 0xed, 0xad, 0xbf, 0xed, 0xbf, 0xbf }, // U+DB7F U+DFFF
        { 0xed, 0xae, 0x80, 0xed, 0xb0, 0x80 }, // U+DB80 U+DC00
        { 0xed, 0xae, 0x80, 0xed, 0xbf, 0xbf }, // U+DB80 U+DFFF
        { 0xed, 0xaf, 0xbf, 0xed, 0xb0, 0x80 }, // U+DBFF U+DC00
        { 0xed, 0xaf, 0xbf, 0xed, 0xbf, 0xbf }, // U+DBFF U+DFFF
    };

    for (const auto &t: DOUBLE)
        tap.expect_throw<util::utf8::error> (
            [&t] () { util::utf8::decode (util::make_view (t)); },
            "reject utf16 paired surrogate"
        );


    static const std::array<uint8_t,3> OTHER[] = {
        { 0xef, 0xbf, 0xbe }, // FFFE
        { 0xef, 0xbf, 0xbf }, // FFFF
    };


    for (const auto &t: OTHER)
        tap.expect_throw<util::utf8::error> (
            [&t] () { util::utf8::decode (util::make_view (t)); },
            "reject BOM"
        );


};


///////////////////////////////////////////////////////////////////////////////
int
main()
{
    util::TAP::logger tap;

    simple_valid (tap);
    single_boundaries (tap);
    malformed (tap);
    overlong (tap);
    illegal (tap);

    return tap.status ();
};