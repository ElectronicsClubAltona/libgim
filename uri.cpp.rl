/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "uri.hpp"

#include "debug.hpp"
#include "iterator.hpp"

#include <algorithm>
#include <iostream>


%%{
    machine impl;

    action trace { if (0) std::cerr << *p; }
    action success {__success = true; }
    action failure {__success = false; }

    action scheme_begin { m_views[SCHEME] = { p, p }; }
    action scheme_end   { m_views[SCHEME] = { m_views[SCHEME].begin (), p }; }

    action hier_begin { m_views[HIERARCHICAL] = { p, p }; }
    action hier_end   { m_views[HIERARCHICAL] = { m_views[HIERARCHICAL].begin (), p }; }

    action user_begin { m_views[USER] = { p, p }; }
    action user_end   { m_views[USER] = { m_views[USER].begin (), p }; }

    action host_begin { m_views[HOST] = { p, p }; }
    action host_end   { m_views[HOST] = { m_views[HOST].begin (), p }; }

    action port_begin { m_views[PORT] = { p, p }; }
    action port_end   { m_views[PORT] = { m_views[PORT].begin (), p }; }

    action authority_begin { m_views[AUTHORITY] = { p, p}; }
    action authority_end   { m_views[AUTHORITY] = { m_views[AUTHORITY].begin (), p }; }

    action path_begin { m_views[PATH] = { p, p}; }
    action path_end   { m_views[PATH] = { m_views[PATH].begin (), p }; }

    action query_begin { m_views[QUERY] = { p, p}; }
    action query_end   { m_views[QUERY] = { m_views[QUERY].begin (), p }; }

    action fragment_begin { m_views[FRAGMENT] = { p, p}; }
    action fragment_end   { m_views[FRAGMENT] = { m_views[FRAGMENT].begin (), p }; }

    action uri_begin {}
    action uri_end {}

    include rfc3986 'rfc3986.rl';

    impl := URI
    %success
    $!failure
    $trace;

    write data;
}%%


///////////////////////////////////////////////////////////////////////////////
// URI

util::uri::uri (const char *str):
    uri (std::string (str))
{ ; }


//-----------------------------------------------------------------------------
util::uri::uri (const char *first, const char *last):
    uri (std::string (first, last))
{ ; }


///////////////////////////////////////////////////////////////////////////////
static const util::view<const char*> NULL_VIEW { nullptr, nullptr };


//-----------------------------------------------------------------------------
util::uri::uri (std::string &&_value):
    m_views {
        NULL_VIEW,
        NULL_VIEW,
        NULL_VIEW,
        NULL_VIEW,
        NULL_VIEW,
        NULL_VIEW,
        NULL_VIEW,
        NULL_VIEW,
        NULL_VIEW
    },
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
util::view<const char*>
util::uri::get (util::uri::component c) const
{
    CHECK_NEQ (c, NUM_COMPONENTS);
    return m_views[c];
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
util::uri::percent_decode (view<const char*> s)
{
    if (s.size () == 0)
        return std::string ();

    // Early check for late percent-encoding so we can simplify the decode loop
    {
        auto tail = std::find (s.size () < 3 ? s.begin ()
                                             : s.end () - 2,
                               s.end (),
                               '%');
        if (tail != s.end ())
            throw parse_error ("triple overlaps end");
    }

    // Allocate and size a potentially overlong output string. This allows us
    // to copy directly into its buffer. We'll shorten it at the end.
    std::string out;
    out.resize (s.size ());

    // Find the percent, copy until that, decode, advance, repeat.
    auto out_cursor = out.begin ();

    for (auto i = s.begin (); i < s.end (); ++i) {
        auto cursor = std::find (i, s.end (), '%');

        if (cursor == s.end ()) {
            out_cursor = std::copy (i, s.end (), out_cursor);
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
util::operator<< (std::ostream &os, util::uri::component c)
{
    switch (c) {
        case util::uri::SCHEME:         return os << "SCHEME";
        case util::uri::HIERARCHICAL:   return os << "HIERARCHICAL";
        case util::uri::AUTHORITY:      return os << "AUTHORITY";
        case util::uri::USER:           return os << "USER";
        case util::uri::HOST:           return os << "HOST";
        case util::uri::PORT:           return os << "PORT";
        case util::uri::PATH:           return os << "PATH";
        case util::uri::QUERY:          return os << "QUERY";
        case util::uri::FRAGMENT:       return os << "FRAGMENT";

        case util::uri::NUM_COMPONENTS:
            unreachable ();
    }

    unreachable ();
}
