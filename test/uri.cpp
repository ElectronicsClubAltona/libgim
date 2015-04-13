#include "uri.hpp"

#include "debug.hpp"
#include "tap.hpp"

int
main (void)
{
    static const struct {
        const char *src;
        const char *scheme;
        const char *authority;
        const char *path;
        const char *query;
        const char *fragment;
    } GOOD[] = {
        { "ftp://ftp.is.co.za/rfc/rfc1808.txt", "ftp", "ftp.is.co.za", "/rfc/rfc1808.txt", "", "" },
        { "http://www.ietf.org/rfc/rfc2396.txt", "http", "www.ietf.org", "/rfc/rfc2396.txt", "", "" },
        { "ldap://[2001:db8::7]/c=GB?objectClass?one", "ldap", "[2001:db8::7]", "/c=GB", "objectClass?one", "" },
        { "mailto:John.Doe@example.com", "mailto", "", "John.Doe@example.com", "", "" },
        { "news:comp.infosystems.www.servers.unix", "news", "", "comp.infosystems.www.servers.unix", "", "" },
        { "tel:+1-816-555-1212", "tel", "", "+1-816-555-1212", "", "" },
        { "telnet://192.0.2.16:80/", "telnet", "192.0.2.16:80", "/", "", "" },
        { "urn:oasis:names:specification:docbook:dtd:xml:4.1.2", "urn", "", "oasis:names:specification:docbook:dtd:xml:4.1.2", "", "" },
    };

    for (auto i: GOOD) {
        CHECK_NOTHROW (util::uri foo (i.src));

        util::uri u (i.src);

        CHECK_EQ (i.scheme,     u.get (util::uri::SCHEME));
        CHECK_EQ (i.authority,  u.get (util::uri::AUTHORITY));
        CHECK_EQ (i.path,       u.get (util::uri::PATH));
        CHECK_EQ (i.query,      u.get (util::uri::QUERY));
        CHECK_EQ (i.fragment,   u.get (util::uri::FRAGMENT));
    }

    static const char* BAD[] = {
        "www.google.com.au",
    };

    for (auto i: BAD)
        CHECK_THROWS (util::uri::parse_error, util::uri foo (i));

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}
