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
    class version {
        public:
            enum release_t {
                RELEASE_ALPHA,
                RELEASE_BETA,
                RELEASE_GAMMA,
                RELEASE_PRODUCTION
            };

            version (unsigned int _major,
                     unsigned int _minor);
            version (const std::string& str);
            version (const char *str);

            virtual ~version () { ; }

            virtual void sanity (void) const;

        protected:
            enum {
                OFFSET_MAJOR = 0,
                OFFSET_MINOR = 1,
                OFFSET_POINT = 2,
                OFFSET_BUILD = 3,

                NUM_OFFSETS
            };

            std::array<unsigned int, NUM_OFFSETS> m_values;
            size_t m_size;
            release_t m_release;

            void parse (const std::string&);
            void parse (const char*);

        public:
            unsigned int major (void) const { return m_values[OFFSET_MAJOR]; }
            unsigned int minor (void) const { return m_values[OFFSET_MINOR]; }
            unsigned int point (void) const { return m_values[OFFSET_POINT]; }
            unsigned int build (void) const { return m_values[OFFSET_BUILD]; }

            size_t size (void) const { return m_size; }

            bool operator <  (const version& rhs) const;
            bool operator >  (const version& rhs) const;
            bool operator >= (const version& rhs) const;
            bool operator <= (const version& rhs) const;
            bool operator == (const version& rhs) const
                { return m_values  == rhs.m_values &&
                         m_release == rhs.m_release; }

            friend std::ostream&
            operator <<(std::ostream& os, const version& rhs);
    };
}


#endif // __VERSION_HPP
