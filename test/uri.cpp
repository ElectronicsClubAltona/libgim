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
        const char *hierarchical;
        const char *authority;
        const char *user;
        const char *host;
        const char *port;
        const char *path;
        const char *query;
        const char *fragment;
    } GOOD[] = {
        // examples from rfc3986
        {
            .src = "ftp://ftp.is.co.za/rfc/rfc1808.txt",

            .scheme = "ftp",
            .hierarchical = "ftp.is.co.za/rfc/rfc1808.txt",
            .authority = "ftp.is.co.za",
            .user = "",
            .host = "ftp.is.co.za",
            .port = "",
            .path = "/rfc/rfc1808.txt",
            .query = "",
            .fragment = "" },
        {
            .src = "http://www.ietf.org/rfc/rfc2396.txt",

            .scheme = "http",
            .hierarchical = "www.ietf.org/rfc/rfc2396.txt",
            .authority = "www.ietf.org",
            .user = "",
            .host = "www.ietf.org",
            .port = "",
            .path = "/rfc/rfc2396.txt",
            .query = "",
            .fragment = "" },

        {
            .src = "ldap://[2001:db8::7]/c=GB?objectClass?one",

            .scheme = "ldap",
            .hierarchical = "[2001:db8::7]/c=GB",
            .authority = "[2001:db8::7]",
            .user = "",
            .host = "[2001:db8::7]",
            .port = "",
            .path = "/c=GB",
            .query = "objectClass?one",
            .fragment = "" },

        {
            .src = "mailto:John.Doe@example.com",

            .scheme= "mailto",
            .hierarchical = "John.Doe@example.com",
            .authority= "",
            .user = "",
            .host = "",
            .port = "",
            .path= "John.Doe@example.com",
            .query= "",
            .fragment= "" },

        {
            .src  = "news:comp.infosystems.www.servers.unix",
            .scheme= "news",
            .hierarchical = "comp.infosystems.www.servers.unix",
            .authority= "",
            .user = "",
            .host = "",
            .port = "",
            .path= "comp.infosystems.www.servers.unix",
            .query= "",
            .fragment= "" },

        {
            .src  = "tel:+1-816-555-1212",

            .scheme= "tel",
            .hierarchical = "+1-816-555-1212",
            .authority= "",
            .user = "",
            .host = "",
            .port = "",
            .path= "+1-816-555-1212",
            .query= "",
            .fragment= "" },

        {
            .src  = "telnet://192.0.2.16:80/",

            .scheme= "telnet",
            .hierarchical = "192.0.2.16:80/",
            .authority= "192.0.2.16:80",
            .user = "",
            .host = "192.0.2.16",
            .port = "80",
            .path= "/",
            .query= "",
            .fragment= "" },

        {
            .src  = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2",

            .scheme= "urn",
            .hierarchical = "oasis:names:specification:docbook:dtd:xml:4.1.2",
            .authority= "",
            .user = "",
            .host = "",
            .port = "",
            .path= "oasis:names:specification:docbook:dtd:xml:4.1.2",
            .query= "",
            .fragment= "" },


        // a case with all possible components
        {
            .src = "https://user:password@example.com:80/path/to?foo=bar#fragment",

            .scheme = "https",
            .hierarchical = "user:password@example.com:80/path/to",
            .authority = "user:password@example.com:80",
            .user = "user:password",
            .host = "example.com",
            .port = "80",
            .path = "/path/to",
            .query = "foo=bar",
            .fragment = "fragment" },
    };

    for (auto t: GOOD) {
        tap.expect_nothrow ([t] (void) { util::uri foo (t.src); }, "nothrow parsing '%s'", t.src);
        util::uri u (t.src);

        tap.expect (equal (u.scheme (), t.scheme), "scheme for '%s'", t.src);
        tap.expect (equal (u.heirarchical (), t.hierarchical), "hierarchical for '%s'", t.src);
        tap.expect (equal (u.authority (), t.authority), "authority for '%s'", t.src);
        tap.expect (equal (u.host (), t.host), "host for '%s'", t.src);
        tap.expect (equal (u.user (), t.user), "user for '%s'", t.src);
        tap.expect (equal (u.port (), t.port), "port for '%s'", t.src);
        tap.expect (equal (u.path (), t.path), "path for '%s'", t.src);
        tap.expect (equal (u.query (), t.query), "query for '%s'", t.src);
        tap.expect (equal (u.fragment (), t.fragment), "fragment for '%s'", t.src);
    }

    static const char* BAD[] = {
        "www.google.com.au",
    };

    for (auto i: BAD)
        tap.expect_throw<util::uri::parse_error> (
            [i] (void) { util::uri foo (i); }, "throw parsing '%s'", i
    );


    return tap.status ();
}
