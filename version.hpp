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

#ifndef __UTIL_VERSION_HPP
#define __UTIL_VERSION_HPP

#include <array>
#include <string>
#include <iostream>


namespace util {
    struct version {
        enum release_t {
            RELEASE_ALPHA,
            RELEASE_BETA,
            RELEASE_GAMMA,
            RELEASE_PRODUCTION
        };

        version ();
        version (unsigned int _major, unsigned int _minor);
        version (const std::string& str);
        version (const char *str);

        void sanity (void) const;

        enum {
            OFFSET_MAJOR = 0,
            OFFSET_MINOR = 1,
            OFFSET_POINT = 2,
            OFFSET_BUILD = 3,

            NUM_OFFSETS
        };

        unsigned major (void) const { return values[OFFSET_MAJOR]; }
        unsigned minor (void) const { return values[OFFSET_MINOR]; }
        unsigned point (void) const { return values[OFFSET_POINT]; }
        unsigned build (void) const { return values[OFFSET_BUILD]; }

        std::array<unsigned int, NUM_OFFSETS> values;
        size_t size;
        release_t release;

        static version parse (const std::string&);
        static version parse (const char*);

        bool operator <  (const version& rhs) const;
        bool operator >  (const version& rhs) const;
        bool operator >= (const version& rhs) const;
        bool operator <= (const version& rhs) const;
        bool operator == (const version& rhs) const;
    };
}

std::ostream& operator<< (std::ostream& os, const util::version& rhs);


#endif // __VERSION_HPP
