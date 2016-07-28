#include "version.hpp"

#include "tap.hpp"


///////////////////////////////////////////////////////////////////////////////
static const struct {
    const char *msg;
    const char *str;
    unsigned parts[4];
    util::version::release_t release;

} PARSE_TESTS[] = {
    { "parse 1 component",        "1",          { 1u             }, util::version::PRODUCTION },

    { "parse 2component",        "1.2",        { 1u, 2u         }, util::version::PRODUCTION },
    { "parse 3component",        "1.2.3",      { 1u, 2u, 3u     }, util::version::PRODUCTION },
    { "parse 4component",        "1.2.3.4",    { 1u, 2u, 3u, 4u }, util::version::PRODUCTION },

    { "parse 2 component alpha",  "9.5a",       { 9u, 5u         }, util::version::ALPHA },
    { "parse 3 component beta",   "8.2.5b",     { 8u, 2u, 5u     }, util::version::BETA },

    /*
    { "1.4.1-p8",   { 1, 4, 1    } },
    { "4.2.0-r4",   { 4, 2, 0    } },

    { "1.4 RC1",    { 1, 4       } }
    */
};


///////////////////////////////////////////////////////////////////////////////
static const struct {
    util::version a, b;
    bool eq, lt, gt;
    const char *msg;
} CMP_TESTS[] = {
    { { 1u     }, { 1u     }, true,  false, false, "v1 cmp v1" },
    { { 1u     }, { 1u, 0u }, true,  false, false, "v1 cmp v1.0" },
    { { 1u, 0u }, { 1u     }, true,  false, false, "v1.0 cmp v1" },
    { { 1u, 1u }, { 1u     }, false, false, true,  "v1.1 cmp v1" },
    { { 1u,    }, { 1u, 1u }, false, true,  false, "v1 cmp v1.1" },
};


///////////////////////////////////////////////////////////////////////////////
int
main () {

    util::TAP::logger tap;

    for (const auto &i: PARSE_TESTS) {
        util::version v (i.str);

        tap.expect (std::equal (v.begin (), v.end (), i.parts) && v.release == i.release, "%s", i.msg);
    }


    for (const auto &t: CMP_TESTS) {
        bool eq = t.a == t.b,
             lt = t.a <  t.b,
             gt = t.a >  t.b;

        tap.expect (t.eq == eq, "%s: equality", t.msg);
        tap.expect (t.lt == lt, "%s: less-than", t.msg);
        tap.expect (t.gt == gt, "%s: greater-than", t.msg);
    }

    return tap.status ();
}
