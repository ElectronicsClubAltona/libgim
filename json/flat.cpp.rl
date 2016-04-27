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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "json/flat.hpp"

#include "json/except.hpp"

#include "io.hpp"
#include "debug.hpp"

#include <deque>

//-----------------------------------------------------------------------------
%%{
    # JSON (rfc7159)
    machine json;

    action trace { if (false) std::cerr << *p; }
    action success { __success = true; }
    action failure { }

    action new_line { ++line; }

    action first { parsed.push_back ({ type::UNKNOWN, p, p}); }
    action last  { parsed.back ().last  = p; }

    action tag_nul     { parsed.back ().tag = type::NUL; }
    action tag_boolean { parsed.back ().tag = type::BOOLEAN; }
    action tag_string  { parsed.back ().tag = type::STRING; }
    action tag_integer { parsed.back ().tag = type::INTEGER; }
    action tag_real    { parsed.back ().tag = type::REAL; }

    action tag_object_begin { parsed.push_back ({ type::OBJECT_BEGIN,   p, p + 1 }); }
    action tag_object_end   { parsed.push_back ({ type::OBJECT_END,     p, p + 1 }); }
    action tag_array_begin  { parsed.push_back ({ type::ARRAY_BEGIN,    p, p + 1 }); }
    action tag_array_end    { parsed.push_back ({ type::ARRAY_END,      p, p + 1 }); }

    # Line counter
    lines = (
          any | '\n' @new_line
    )*;

    # UTF-8 (rfc3629)
    utf8_tail = 0x80..0xbf;

    utf8_1 =      0x00..0x7f;
    utf8_2 =      0xc2..0xdf utf8_tail;
    utf8_3 = 0xe0 0xa0..0xbf utf8_tail      |
                  0xe1..0xec utf8_tail{2}   |
             0xed 0x80..0x9f utf8_tail      |
                  0xee..0xef utf8_tail{2};
    utf8_4 = 0xf0 0x90..0xbf utf8_tail{2}   |
                  0xf1..0xf3 utf8_tail{3}   |
             0xf4 0x80..0x8f utf8_tail{2};


    utf8 = utf8_1 | utf8_2 | utf8_3 | utf8_4;

    # Utility
    ws = 0x20 | 0x09 | 0x0A | 0x0D;
    array_start  = '[';
    array_end    = ']';
    object_start = '{';
    object_end   = '}';

    # Strings
    char =
          (utf8 - ["\\])
        | "\\" (
              [\\"/bfnrt]
            | "u" xdigit{4}
        )
    ;

    string = ('"' char* '"') >first >tag_string %*last;

    # numbers
    int = '0' | [1-9] digit*;

    frac = '.' digit+;
    e    = 'e'i[+\-]?;
    exp  = e digit+;

    number = (
        '-'?
        int
        (frac >tag_real)?
        exp?
    ) >tag_integer;

    # wrapper types
    array  = array_start  @{ fhold; fcall array_members;  } array_end;
    object = object_start @{ fhold; fcall object_members; } object_end;

    # simple types; case sensitive literals
    bool = ("true" | "false") >tag_boolean;
    nul  = "null" >tag_nul;
    literal = bool | nul;

    value = object | array | (number | string | literal) >first %last;

    # Complex
    member  = string ws* ':' ws* value;

    array_members  := ((
        array_start >tag_array_begin ws* (value  ws* (',' ws* value  ws*)*)? array_end >tag_array_end
    ) & lines)
    @{ fhold; fret; } $trace $!failure;

    object_members := ((
        object_start >tag_object_begin ws* (member ws* (',' ws* member ws*)*)? object_end >tag_object_end
    ) & lines)
    @{ fhold; fret; } $trace $!failure;

    # meta types
    document := ((ws* value ws*) & lines)
    %success
    $!failure
    $trace;

    variable stack ragelstack;
    prepush { ragelstack.push_back (0); }
    postpop { ragelstack.pop_back (); }

    write data;
}%%


//-----------------------------------------------------------------------------
std::vector<json::flat::item>
json::flat::parse (const char *first, const char *last)
{
    const char *p   = first;
    const char *pe  = last;
    const char *eof = pe;

    std::deque<int> ragelstack;
    std::vector<item> parsed;

    size_t line = 0;
    int cs, top;
    bool __success = false;

    %%write init;
    %%write exec;

    if (!__success)
        throw json::parse_error ("parse error", line);

    return parsed;
}


//-----------------------------------------------------------------------------
std::vector<json::flat::item>
json::flat::parse (const boost::filesystem::path &path)
{
    util::mapped_file f (path.string ().c_str ());
    if (f.empty ())
        throw parse_error ("empty file");

    return parse ((const char *)f.cbegin (), (const char*)f.cend ());
}


//-----------------------------------------------------------------------------
std::ostream&
json::flat::operator<< (std::ostream &os, json::flat::type t)
{
    switch (t) {
    case json::flat::type::STRING:  os << "STRING";     break;
    case json::flat::type::NUL:     os << "NUL";        break;
    case json::flat::type::BOOLEAN: os << "BOOLEAN";    break;
    case json::flat::type::INTEGER: os << "INTEGER";    break;
    case json::flat::type::REAL:    os << "REAL";       break;

    case json::flat::type::OBJECT_BEGIN:    os << "OBJECT_BEGIN";   break;
    case json::flat::type::OBJECT_END:      os << "OBJECT_END";     break;
    case json::flat::type::ARRAY_BEGIN:     os << "ARRAY_BEGIN";    break;
    case json::flat::type::ARRAY_END:       os << "ARRAY_END";      break;

    default:
        unreachable ();
    }

    return os;
}

