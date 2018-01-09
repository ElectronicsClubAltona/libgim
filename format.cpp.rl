/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#include "./format.hpp"

#include <iostream>

namespace util::format {
    std::ostream&
    operator<< (std::ostream &os, type_t val)
    {
        switch (val) {
        case type_t::LITERAL:   return os << "LITERAL";
        case type_t::USER:      return os << "USER";
        case type_t::ESCAPE:    return os << "ESCAPE";
        case type_t::SIGNED:    return os << "SIGNED";
        case type_t::UNSIGNED:  return os << "UNSIGNED";
        case type_t::REAL:      return os << "REAL";
        case type_t::STRING:    return os << "STRING";
        case type_t::CHAR:      return os << "CHAR";
        case type_t::POINTER:   return os << "POINTER";
        case type_t::COUNT:     return os << "COUNT";
        }

        return os << "UNKNOWN_" << static_cast<std::underlying_type_t<type_t>> (val);
    }


    std::ostream&
    operator<< (std::ostream &os, const specifier &val)
    {
        return os << "{ fmt: " << val.fmt << ", type: " << val.type << " }";
    }
}


///////////////////////////////////////////////////////////////////////////////
%%{
    machine printf;

    parameter = digit+ '$';

    flag =   '+' %{ s.flags.plus  = true; }
           | '-' %{ s.flags.minus = true; }
           | ' ' %{ s.flags.space = true; }
           | '0' %{ s.flags.zero  = true; }
           | '#' %{ s.flags.hash  = true; }
    ;

    width = digit+ >{ s.width = 0; } ${ s.width *= 10; s.width += fc - '0'; };

    # precision may be zero digits which implies zero precision.
    precision = '.' >{ s.precision = 0; } digit* ${ s.precision *= 10; s.precision += fc - '0'; };

    length =
          'hh' %{ s.length = sizeof (char);  }
        | 'h'  %{ s.length = sizeof (short); }
        | 'l'  %{ s.length = sizeof (long);  }
        | 'll' %{ s.length = sizeof (long long); }
        | 'L'  %{ s.length = sizeof (long double); }
        | 'z'  %{ s.length = sizeof (size_t); }
        | 'j'  %{ s.length = sizeof (intmax_t); }
        | 't'  %{ s.length = sizeof (ptrdiff_t); }
    ;

    type = (
             '!' >{ s.type = type_t::USER;   }
           | '%' >{ s.type = type_t::ESCAPE; }
           | (
               'd'
             | 'i'
           ) >{ s.type = type_t::SIGNED; }
           | (
               'u'
             | 'x' %{ s.base = 16; }
             | 'X' %{ s.base = 16; s.upper = true; }
             | 'o' %{ s.base =  8; }
           ) >{ s.type = type_t::UNSIGNED; }
           | (
               ('f' | 'F' %{ s.upper = true; }) %{ s.representation = specifier::FIXED; }
             | ('e' | 'E' %{ s.upper = true; }) %{ s.representation = specifier::SCIENTIFIC; }
             | ('g' | 'G' %{ s.upper = true; }) %{ s.representation = specifier::DEFAULT; }
             | ('a' | 'A' %{ s.upper = true; }) %{ s.representation = specifier::HEX; s.base = 16; }
           ) >{ s.type = type_t::REAL; }
           | 's' >{ s.type = type_t::STRING; }
           | 'c' >{ s.type = type_t::CHAR; }
           | 'p' >{ s.type = type_t::POINTER; }
           | 'n' >{ s.type = type_t::COUNT; }
    );

    literal = ([^%]+)
        >{
            s = specifier {};
            s.fmt = {fpc,fpc};
            s.type = type_t::LITERAL;
        }
        %{
            s.fmt = {s.fmt.begin(),fpc};
            if (!s.fmt.empty ()) {
                specs.push_back (s);
            }
        };

    specifier = (
        '%'
        parameter?
        flag**
        width?
        precision?
        length?
        type
    )
    >{
        s = specifier {};
        s.fmt = {fpc,fpc};
    }
    %{
        s.fmt = {s.fmt.begin(),fpc};
        specs.push_back (s);
    };

    format := literal? (specifier literal?)**
    >{ success = false; }
    %{ success = true;  }
    ;

    write data;
}%%


///////////////////////////////////////////////////////////////////////////////
util::format::parsed
util::format::printf (util::view<const char*> fmt)
{
    std::vector<specifier> specs;
    specifier s;
    bool success = false;
    (void)s;

    int cs;

    auto p   = std::cbegin (fmt);
    auto pe  = std::cend   (fmt);
    auto eof = pe;

    %%write init;
    %%write exec;

    if (!success)
        throw std::runtime_error ("invalid format specification");

    return parsed { std::move (specs) };
}


/// parses a format specifier in the style of PEP3101 (with the notable
/// exception of named parameters).
///
/// in the event of a parsing error the function will throw. makes no
/// attempt to cater for constexpr validation.
util::format::parsed
util::format::python (util::view<const char*> fmt)
{
    std::vector<specifier> specs;

    const auto *prev   = std::begin (fmt);
    const auto *cursor = std::begin (fmt);
    while (*cursor) {
        switch (*cursor) {
            case '{':
            {
                {
                    specifier s;
                    s.fmt = { prev, cursor };
                    s.type = type_t::LITERAL;
                    specs.push_back (s);
                }

                auto first = cursor;
                while (*++cursor != '}')
                    ;

                ++cursor;
                {
                    specifier s;
                    s.fmt = {first, cursor};
                    s.type = type_t::USER;
                    specs.push_back (s);
                }

                prev = cursor;
                break;
            }

            default:
                ++cursor;
                break;
        }
    }

    {
        specifier s;
        s.fmt = {prev,cursor};
        s.type = type_t::LITERAL;
        specs.push_back (s);
    }

    return parsed {std::move (specs)};
}
