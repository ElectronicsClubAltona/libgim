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
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */


#include "version.hpp"

#include <stdexcept>

#include "debug.hpp"


using namespace std;


version::version (unsigned int _major,
                  unsigned int _minor):
        m_values  (2),
        m_release (RELEASE_PRODUCTION) {
    m_values[OFFSET_MAJOR] = _major;
    m_values[OFFSET_MINOR] = _minor;
}


version::version (const string& str):
        m_values  (NUM_OFFSETS, 0),
        m_release (RELEASE_PRODUCTION) {
    m_values.clear ();
    parse (str);
}


static void
check_release (version::release_t r) {
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
    check_release (m_release);
    check (!m_values.empty ());
}


%%{
    machine version;
    
    action clear 
        { current = 0; }
    action increment 
        { current *= 10;
          current += (uintptr_t)(fc - (unsigned char)'0'); }
    action finish
        { m_values.push_back (current); }

    number   = (digit+)
               >clear
               $increment
               %finish;
                  
    dots     = (number '.')* number;

    type     =   ('beta'i     | 'b'i) %{ m_release = RELEASE_BETA;  }
               | ('alpha'i    | 'a'i) %{ m_release = RELEASE_ALPHA; }
               | ('gamma'i    | 'g'i) %{ m_release = RELEASE_GAMMA; };

    version := (dots type?)
               $!{ throw invalid_argument (str); };

	write data;
}%%


void
version::parse (const string& str) {
    unsigned int current;

    int         cs;
    const char *p   = str.data (),
               *pe  = str.data () + str.size (),
               *eof = pe;

    %%write init;
    %%write exec;
}


static string
release_string (const version::release_t r) {
    switch (r) {
        case (version::RELEASE_ALPHA):        return "a";
        case (version::RELEASE_BETA):         return "b";
        case (version::RELEASE_GAMMA):        return "g";
        case (version::RELEASE_PRODUCTION):   return  "";
    }

    panic ("invalid release_t");
}


bool
version::operator > (const version &rhs) const {
    unsigned int count = min (m_values.size (), rhs.m_values.size ());

    for (unsigned int i = 0; i < count; ++i)
        if (m_values[i] < rhs.m_values[i])
            return false;

    if (m_values.size () < rhs.m_values.size())
        return false;

    if (m_release <= rhs.m_release)
        return false;

    return true;
}


ostream&
operator <<(ostream& os, const version& rhs) {
    auto i = rhs.m_values.begin();
    os << *i; ++i;

    for (; i != rhs.m_values.end(); ++i)
        os << '.' << *i;

    os << release_string (rhs.m_release);
    return os;
}

