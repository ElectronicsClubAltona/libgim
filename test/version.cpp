#include "version.hpp"

#include "tap.hpp"

static const struct {
    const char *msg;
    const char *str;
    unsigned parts[4];
    util::version::release_t release;

} TESTS[] = {
    { "1-component",        "1",          { 1u             }, util::version::PRODUCTION },

    { "2-component",        "1.2",        { 1u, 2u         }, util::version::PRODUCTION },
    { "3-component",        "1.2.3",      { 1u, 2u, 3u     }, util::version::PRODUCTION },
    { "4-component",        "1.2.3.4",    { 1u, 2u, 3u, 4u }, util::version::PRODUCTION },

    { "2-component alpha",  "9.5a",       { 9u, 5u         }, util::version::ALPHA },
    { "3-component beta",   "8.2.5b",     { 8u, 2u, 5u     }, util::version::BETA },

    /*
    { "1.4.1-p8",   { 1, 4, 1    } },
    { "4.2.0-r4",   { 4, 2, 0    } },

    { "1.4 RC1",    { 1, 4       } }
    */
};

//-----------------------------------------------------------------------------
int
main () {

    util::TAP::logger tap;

    for (const auto &i: TESTS) {
        util::version v (i.str);

        tap.expect (std::equal (v.begin (), v.end (), i.parts) && v.release == i.release,
                    i.msg);
    }

    return tap.status ();
}
