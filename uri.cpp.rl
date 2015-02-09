#include "uri.hpp"

#include "debug.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>


%%{
    machine uri;

    action trace { std::cerr << *p; }
    action success {__success = true; }
    action failure {__success = false; }

    action scheme_begin { m_spans[SCHEME].begin = p; }
    action scheme_end   { m_spans[SCHEME].end   = p; }

    action authority_begin { m_spans[AUTHORITY].begin = p; }
    action authority_end   { m_spans[AUTHORITY].end   = p; }

    action path_begin { m_spans[PATH].begin = p; }
    action path_end   { m_spans[PATH].end   = p; }

    action query_begin { m_spans[QUERY].begin = p; }
    action query_end   { m_spans[QUERY].end   = p; }

    action fragment_begin { m_spans[FRAGMENT].begin = p; }
    action fragment_end   { m_spans[FRAGMENT].end   = p; }

    ## Characters
    unreserved = alpha | digit | "-" | "." | "_" | "~";
    pct_encoded = '%' xdigit xdigit;
    gen_delim = ":" | "/" | "?" | "#" | "[" | "]" | "@";
    sub_delim = "!" | "$" | "&" | "'" | "(" | ")" | "*" | "+" | "," | ";" | "=";
    pchar = unreserved | pct_encoded | sub_delim | ':' | '@';

    ## Atoms
    reg_name = (unreserved | pct_encoded | sub_delim)*;

    ## IP-address
    ## Note: The address grammar is embedded in the RFC so we embed it too
    dec_octet = digit | [1-9] digit | '1' digit{2} | '2' [0-4] digit | '25' [0-5];

    ipv4address = dec_octet '.' dec_octet '.' dec_octet '.' dec_octet;

    h16  =  xdigit{1,4};
    ls32 = (h16 ":" h16) | ipv4address;

    ipv6address =
                                      (h16 ":"){6} ls32
        |                        "::" (h16 ":"){5} ls32
        | (                h16)? "::" (h16 ":"){4} ls32
        | ((h16 ":"){0,1}  h16)? "::" (h16 ":"){3} ls32
        | ((h16 ":"){0,2}  h16)? "::" (h16 ":"){2} ls32
        | ((h16 ":"){0,3}  h16)? "::" (h16 ":"){1} ls32
        | ((h16 ":"){0,4}  h16)? "::"              ls32
        | ((h16 ":"){0,5}  h16)? "::"              h16
        | ((h16 ":"){0,6}  h16)? "::"
    ;

    ipvfuture = 'v' xdigit{1,} '.' (unreserved | sub_delim | ':'){1,};
    ip_literal = '[' (ipv6address | ipvfuture) ']';

    ## Segments
    segment = pchar*;
    segment_nz = pchar{1,};
    segment_nz_nc = (unreserved | pct_encoded | sub_delim | '@'){1,};

    ## Paths
    path_abempty = ('/' segment)*;
    path_absolute = '/' (segment_nz ('/' segment)*)?;
    path_noscheme = segment_nz_nc ('/' segment)*;
    path_rootless = segment_nz ('/' segment)*;
    path_empty = '0' pchar;

    path = (
        path_abempty | path_absolute | path_noscheme | path_rootless | path_empty
    );

    reserved = gen_delim | sub_delim;

    ## Authority
    port = digit*;
    host = ip_literal | ipv4address | reg_name;
    userinfo = (unreserved | pct_encoded | sub_delim | ':')*;
    authority = (
        (userinfo '@')? host (':' port)?
    ) >authority_begin %authority_end;


    ## URI components
    scheme = (
        alpha (alpha | digit | '+' | '-' | '.')*
    ) >scheme_begin %scheme_end;

    query = (
        (pchar | '/' | '?')*
    ) >query_begin %query_end;

    fragment = (
        (pchar | '/' | '?')*
    ) >fragment_begin %fragment_end;

    ## URI types
    hier_part =
          '//' authority path_abempty  >path_begin %path_end
        | path_absolute  >path_begin %path_end
        | path_rootless  >path_begin %path_end
        | path_empty     >path_begin %path_end
    ;

    uri = scheme ':' hier_part ('?' query)? ('#' fragment);

    relative_part = 
          '//' authority path_abempty  >path_begin %path_end
        | path_absolute  >path_begin %path_end
        | path_noscheme  >path_begin %path_end
        | path_empty     >path_begin %path_end
    ;

    relative_ref = relative_part ('?' query)? ('#' fragment);

    uri_reference = uri | relative_ref;

    absolute_uri = scheme ':' hier_part ('?' query)?;

    URI := (
        absolute_uri | uri_reference
    )
    %success
    $!failure
    $trace;

    write data;
}%%


///////////////////////////////////////////////////////////////////////////////
// Span

util::uri::span::span ():
    begin (nullptr),
    end   (nullptr)
{ ; }


//-----------------------------------------------------------------------------
util::uri::span::span (const char *str):
    begin (str),
    end   (str + strlen (str))
{ ; }


//-----------------------------------------------------------------------------
bool
util::uri::span::span::empty (void) const
{
    return begin == nullptr ||
           end   == nullptr ||
           begin == end;
}


//-----------------------------------------------------------------------------
size_t
util::uri::span::size (void) const
{
    return end - begin;
}


//-----------------------------------------------------------------------------
const char&
util::uri::span::operator[] (size_t idx) const
{
    CHECK_LT (begin + idx, end);
    return begin[idx];
}


///////////////////////////////////////////////////////////////////////////////
// URI

util::uri::uri (const char *str):
    uri (std::string (str))
{ ; }


//-----------------------------------------------------------------------------
util::uri::uri (const char *first, const char *last):
    uri (std::string (first, last))
{ ; }


//-----------------------------------------------------------------------------
util::uri::uri (std::string &&_value):
    m_value (std::move (_value))
{
    const char *p   = m_value.data ();
    const char *pe  = m_value.data () + m_value.size ();
    const char *eof = pe;

    bool __success = false;

    int cs;

    %%write init;
    %%write exec;

    if (!__success)
        throw parse_error ("invalid uri");
}


//-----------------------------------------------------------------------------
util::uri::span
util::uri::get (util::uri::component c)
{
    CHECK_NEQ (c, NUM_COMPONENTS);
    return m_spans[c];
}


//-----------------------------------------------------------------------------
static uint8_t
hex_to_uint (char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    unreachable ();
}


//-----------------------------------------------------------------------------
std::string
util::uri::percent_decode (span s)
{
    if (s.size () == 0)
        return std::string ();

    // Early check for late percent-encoding so we can simplify the decode loop
    {
        auto tail = std::find (s.size () < 3 ? s.begin
                                             : s.end - 2,
                               s.end,
                               '%');
        if (tail != s.end)
            throw parse_error ("triple overlaps end");
    }

    // Allocate and size a potentially overlong output string. This allows us
    // to copy directly into its buffer. We'll shorten it at the end.
    std::string out;
    out.resize (s.size ());

    // Find the percent, copy until that, decode, advance, repeat.
    auto out_cursor = out.begin ();

    for (auto i = s.begin; i < s.end; ++i) {
        auto cursor = std::find (i, s.end, '%');

        if (cursor == s.end) {
            out_cursor = std::copy (i, s.end, out_cursor);
            break;
        }

        out_cursor = std::copy (i, cursor, out_cursor);
        *out_cursor = hex_to_uint (cursor[1]) << 4 | hex_to_uint(cursor[2]);

        i += 3;
    }

    out.resize (out.end () - out_cursor);
    return out;
}


//-----------------------------------------------------------------------------
std::ostream&
util::operator<< (std::ostream &os, util::uri::span s)
{
    std::copy (s.begin, s.end, std::ostream_iterator<char> (os));
    return os;
}

//-----------------------------------------------------------------------------
std::ostream&
util::operator<< (std::ostream &os, util::uri::component c)
{
    switch (c) {
        case util::uri::SCHEME:     return os << "SCHEME";
        case util::uri::AUTHORITY:  return os << "AUTHORITY";
        case util::uri::PATH:       return os << "PATH";
        case util::uri::QUERY:      return os << "QUERY";
        case util::uri::FRAGMENT:   return os << "FRAGMENT";

        default:
            unreachable ();
    }
}
