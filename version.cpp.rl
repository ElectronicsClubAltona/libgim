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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */


#include "version.hpp"

#include <cstring>
#include <stdexcept>

#include "debug.hpp"


using util::version;


///////////////////////////////////////////////////////////////////////////////
version::version ():
    size (0),
    release (PRODUCTION)
{ ; }


//-----------------------------------------------------------------------------
version::version (unsigned int _major,
                  unsigned int _minor):
    size (0),
    components { _major, _minor },
    release (PRODUCTION)
{
    components[MAJOR] = _major;
    components[MINOR] = _minor;
}


//-----------------------------------------------------------------------------
version::version (const std::string& str): 
    version (str.c_str ())
{ ; }


//-----------------------------------------------------------------------------
version::version (const char *str):
    version ()
{
    *this = version::parse (str);
}


///////////////////////////////////////////////////////////////////////////////
static void
sanity (version::release_t r)
{
    switch (r) {
        case version::ALPHA:
        case version::BETA:
        case version::GAMMA:
        case version::PRODUCTION:
            return;
    }

    panic ("invalid release_t value");
}


//-----------------------------------------------------------------------------
void
version::sanity (void) const {
    ::sanity (release);
    CHECK (size > 0);
}


///////////////////////////////////////////////////////////////////////////////
unsigned version::major (void) const { return components[MAJOR]; }
unsigned version::minor (void) const { return components[MINOR]; }
unsigned version::point (void) const { return components[POINT]; }
unsigned version::build (void) const { return components[BUILD]; }


///////////////////////////////////////////////////////////////////////////////
const unsigned*
version::begin (void) const
{
    return components.begin ();
}


const unsigned*
version::end (void) const
{
    return components.begin () + size;
}


///////////////////////////////////////////////////////////////////////////////
%%{
    machine version;
    
    action clear 
        { accum = 0; }
    action increment 
        { accum *= 10;
          accum += (uintptr_t)(fc - (unsigned char)'0'); }
    action finish
        { v.components[v.size++] = accum ; }

    number   = (digit+)
               >clear
               $increment
               %finish;
                  
    dots     = (number '.')* number;

    type     =   ('beta'i     | 'b'i) %{ v.release = BETA;  }
               | ('alpha'i    | 'a'i) %{ v.release = ALPHA; }
               | ('gamma'i    | 'g'i) %{ v.release = GAMMA; };

    version := (dots type?) >clear
               $!{ throw std::invalid_argument (str); };

    write data;
}%%


//-----------------------------------------------------------------------------
util::version
util::version::parse (const std::string& str) {
    unsigned int accum = 0;

    int         cs;
    const char *p   = str.data (),
               *pe  = str.data () + str.size (),
               *eof = pe;

    version v;

    %%write init;
    %%write exec;
    
    return v;
}


//-----------------------------------------------------------------------------
util::version
util::version::parse (const char *str) {
    return parse (std::string (str));
}


///////////////////////////////////////////////////////////////////////////////
bool
version::operator> (const version &rhs) const
{
    auto count = util::min (size, rhs.size);

    for (decltype(count) i = 0; i < count; ++i)
        if (components[i] < rhs.components[i])
            return false;

    if (size < rhs.size)
        return false;

    if (release <= rhs.release)
        return false;

    return true;
}


//-----------------------------------------------------------------------------
bool
version::operator== (const version &rhs) const {
    return components == rhs.components &&
           size       == rhs.size   &&
           release    == rhs.release;
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    std::ostream&
    operator<< (std::ostream& os, const util::version::release_t r)
    {
        switch (r) {
            case version::ALPHA:        return os << 'a';
            case version::BETA:         return os << 'b';
            case version::GAMMA:        return os << 'g';
            case version::PRODUCTION:   return os;

            default:
                unreachable ();
        }
    }
}


//-----------------------------------------------------------------------------
std::ostream&
util::operator<< (std::ostream& os, const util::version& rhs)
{
    size_t elements = rhs.size;
    CHECK_GT (elements, 0u);

    do {
        os << rhs.major ();
        if (!--elements)
            break;

        os << "." << rhs.minor ();
        if (!--elements)
            break;

        os << "." << rhs.point ();
        if (!--elements)
            break;

        os << "." << rhs.build ();
        CHECK_EQ (elements - 1, 0u);
    } while (0);

    os << rhs.release;

    return os;
}
