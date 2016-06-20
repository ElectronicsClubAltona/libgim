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
version::version (unsigned _major,
                  unsigned _minor,
                  unsigned _point,
                  unsigned _build):
    components { _major, _minor, _point, _build }
{
    size = 4u;
}


//-----------------------------------------------------------------------------
version::version (unsigned _major,
                  unsigned _minor,
                  unsigned _point):
    version (_major, _minor, _point, 0)
{
    size = 3u;
}


//-----------------------------------------------------------------------------
version::version (unsigned _major,
                  unsigned _minor):
    version (_major, _minor, 0)
{
    size = 2u;
}


//-----------------------------------------------------------------------------
version::version (unsigned _major):
    version (_major, 0)
{
    size = 1u;
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
version::begin (void) const noexcept
{
    return components.begin ();
}


//-----------------------------------------------------------------------------
const unsigned*
version::end (void) const noexcept
{
    return components.begin () + size;
}


//-----------------------------------------------------------------------------
const unsigned*
version::cbegin (void) const noexcept
{
    return begin ();
}


//-----------------------------------------------------------------------------
const unsigned*
version::cend (void) const noexcept
{
    return end ();
}


//-----------------------------------------------------------------------------
const unsigned&
version::operator[] (size_t i) const
{
    return components[i];
}


//-----------------------------------------------------------------------------
unsigned&
version::operator[] (size_t i)
{
    return components[i];
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
    unsigned accum = 0;

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
version::operator< (const version &rhs) const noexcept
{
    auto count = util::min (size, rhs.size);

    // make sure each element we have in common is LT
    for (decltype (count) i = 0; i < count; ++i)
        if ((*this)[i] < rhs[i])
            return true;

    // if they have extra elements and they're not zeros then we must be LT
    if (size < rhs.size)
        if (std::none_of (rhs.cbegin () + count, rhs.cend (), [] (auto i) { return i == 0; }))
            return true;

    // test if we have an earlier release schedule
    if (release < rhs.release)
        return true;

    // we're equal or greater to rhs
    return false;
}


//-----------------------------------------------------------------------------
bool
version::operator== (const version &rhs) const noexcept
{
    auto count = util::min (size, rhs.size);

    bool front = std::equal (cbegin (), cbegin () + count, rhs.cbegin ());
    bool left  = std::all_of (this->cbegin () + count, this->cend (), [] (auto i) { return i == 0; });
    bool right = std::all_of (rhs.cbegin () + count, rhs.cend (), [] (auto i) { return i == 0; });

    return front && (size > rhs.size ? left : right) && release == rhs.release;
}


//-----------------------------------------------------------------------------
bool
version::operator<= (const version &rhs) const noexcept
{
    return *this < rhs || *this == rhs;
}


//-----------------------------------------------------------------------------
bool
version::operator> (const version &rhs) const noexcept
{
    return !(*this <= rhs);
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
        }

        unreachable ();
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
