#include "uri.hpp"

#include "debug.hpp"

int
main (void)
{
    static const char* GOOD[] = {
        "ftp://ftp.is.co.za/rfc/rfc1808.txt",
        "http://www.ietf.org/rfc/rfc2396.txt",
        "ldap://[2001:db8::7]/c=GB?objectClass?one",
        "mailto:John.Doe@example.com",
        "news:comp.infosystems.www.servers.unix",
        "tel:+1-816-555-1212",
        "telnet://192.0.2.16:80/",
        "urn:oasis:names:specification:docbook:dtd:xml:4.1.2",
    };

    for (auto i: GOOD)
        CHECK_NOTHROW (util::uri foo (i));
    
    static const char* BAD[] = {
        "www.google.com.au",
    };

    for (auto i: BAD)
        CHECK_THROWS (util::uri::parse_error, util::uri foo (i));
}
