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


using namespace util;

//-----------------------------------------------------------------------------
version::version ():
    size (0)
{ ; }


version::version (unsigned int _major,
                  unsigned int _minor):
    size    (2),
    release (RELEASE_PRODUCTION)
{
    values[OFFSET_MAJOR] = _major;
    values[OFFSET_MINOR] = _minor;
}


version::version (const std::string& str): 
    size (0)
{
    *this = version::parse (str);
}


version::version (const char *str):
    size (0)
{
    *this = version::parse (str);
}


//-----------------------------------------------------------------------------
static void
sanity (version::release_t r) {
    switch (r) {
        case version::RELEASE_ALPHA:
        case version::RELEASE_BETA:
        case version::RELEASE_GAMMA:
        case version::RELEASE_PRODUCTION:
            return;
    }

    panic ("invalid release_t value");
}


void
version::sanity (void) const {
    ::sanity (release);
    CHECK (size > 0);
}


//-----------------------------------------------------------------------------
%%{
    machine version;
    
    action clear 
        { accum = 0; }
    action increment 
        { accum *= 10;
          accum += (uintptr_t)(fc - (unsigned char)'0'); }
    action finish
        { v.values[v.size++] = accum ; }

    number   = (digit+)
               >clear
               $increment
               %finish;
                  
    dots     = (number '.')* number;

    type     =   ('beta'i     | 'b'i) %{ v.release = RELEASE_BETA;  }
               | ('alpha'i    | 'a'i) %{ v.release = RELEASE_ALPHA; }
               | ('gamma'i    | 'g'i) %{ v.release = RELEASE_GAMMA; };

    version := (dots type?)
               $!{ throw std::invalid_argument (str); };

    write data;
}%%


//-----------------------------------------------------------------------------
util::version
util::version::parse (const std::string& str) {
    unsigned int accum;

    int         cs;
    const char *p   = str.data (),
               *pe  = str.data () + str.size (),
               *eof = pe;

    version v;

    %%write init;
    %%write exec;
    
    return v;
}


util::version
util::version::parse (const char *str) {
    return parse (std::string (str));
}


//-----------------------------------------------------------------------------
static std::string
release_to_string (version::release_t r) {
    sanity (r);

    static const char* RELEASE_STRINGS[] = {
        "a",    // RELEASE_ALPHA
        "b",    // RELEASE_BETA
        "g",    // RELEASE_GAMMA
        ""      // RELEASE_PRODUCTION
    };

    return RELEASE_STRINGS[r];
}


bool
version::operator> (const version &rhs) const {
    unsigned int count = min (values.size (), rhs.values.size ());

    for (unsigned int i = 0; i < count; ++i)
        if (values[i] < rhs.values[i])
            return false;

    if (values.size () < rhs.values.size())
        return false;

    if (release <= rhs.release)
        return false;

    return true;
}


bool
version::operator== (const version &rhs) const {
    return values  == rhs.values &&
           size    == rhs.size   &&
           release == rhs.release;
}


std::ostream&
operator <<(std::ostream& os, const util::version& rhs) {
    size_t elements = rhs.size;
    CHECK_HARD (elements > 0);

    os << rhs.major ();
    if (!--elements)
        goto done;

    os << "." << rhs.minor ();
    if (!--elements)
        goto done;

    os << "." << rhs.point ();
    if (!--elements)
        goto done;

    os << "." << rhs.build ();
    CHECK_EQ (--elements, 0);

done:
    os << release_to_string (rhs.release);
    return os;
}
