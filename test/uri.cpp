#include "uri.hpp"

#include "debug.hpp"
#include "tap.hpp"

int
main (void)
{
    util::TAP::logger tap;

    static const struct {
        const char *src;
        const char *scheme;
        const char *authority;
        const char *path;
        const char *query;
        const char *fragment;
    } GOOD[] = {
        { "ftp://ftp.is.co.za/rfc/rfc1808.txt",                     "ftp",      "ftp.is.co.za",     "/rfc/rfc1808.txt", "", "" },
        { "http://www.ietf.org/rfc/rfc2396.txt",                    "http",     "www.ietf.org",     "/rfc/rfc2396.txt", "", "" },
        { "ldap://[2001:db8::7]/c=GB?objectClass?one",              "ldap",     "[2001:db8::7]",    "/c=GB", "objectClass?one", "" },
        { "mailto:John.Doe@example.com",                            "mailto",   "",                 "John.Doe@example.com", "", "" },
        { "news:comp.infosystems.www.servers.unix",                 "news",     "",                 "comp.infosystems.www.servers.unix", "", "" },
        { "tel:+1-816-555-1212",                                    "tel",      "",                 "+1-816-555-1212", "", "" },
        { "telnet://192.0.2.16:80/",                                "telnet",   "192.0.2.16:80",    "/", "", "" },
        { "urn:oasis:names:specification:docbook:dtd:xml:4.1.2",    "urn",      "",                 "oasis:names:specification:docbook:dtd:xml:4.1.2", "", "" },
    };

    for (auto i: GOOD) {
        tap.expect_nothrow ([i] (void) { util::uri foo (i.src); }, "nothrow parsing '%s'", i.src);

        util::uri u (i.src);

        tap.expect (std::equal (u.get (util::uri::SCHEME).begin (), u.get (util::uri::SCHEME).end (), i.scheme),            "extracting scheme for '%s'", i.src);
        tap.expect (std::equal (u.get (util::uri::AUTHORITY).begin (), u.get (util::uri::AUTHORITY).end (), i.authority),   "extracting authority '%s'", i.src);
        tap.expect (std::equal (u.get (util::uri::PATH).begin (), u.get (util::uri::PATH).end (), i.path),                  "extracting path '%s'", i.src);
        tap.expect (std::equal (u.get (util::uri::QUERY).begin (), u.get (util::uri::QUERY).end (), i.query),               "extracting query '%s'", i.src);
        tap.expect (std::equal (u.get (util::uri::FRAGMENT).begin (), u.get (util::uri::FRAGMENT).end (), i.fragment),      "extracting fragment '%s'", i.src);
    }

    static const char* BAD[] = {
        "www.google.com.au",
    };

    for (auto i: BAD)
        tap.expect_throw<util::uri::parse_error> ([i] (void) { util::uri foo (i); }, "throw parsing '%s'", i);


    return tap.status ();
}
