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


#ifndef __UTIL_URI_HPP
#define __UTIL_URI_HPP

#include <string>
#include <cstdlib>
#include <stdexcept>


namespace util {
    class uri {
    public:
        uri (std::string &&);
        uri (const char *str);
        uri (const char *first, const char *last);

        class parse_error : public std::runtime_error
        { using runtime_error::runtime_error; };

        enum component : unsigned {
            SCHEME,
            AUTHORITY,
            PATH,
            QUERY,
            FRAGMENT,

            NUM_COMPONENTS
        };

        struct span {
            span ();
            span (const char*);

            const char *begin;
            const char *end;

            bool empty () const;
            size_t size (void) const;
            const char& operator[] (size_t) const;
        };

        span get (component);

        static std::string percent_decode (span);

    private:
        span m_spans[NUM_COMPONENTS];
        std::string m_value;
    };

    std::ostream& operator<< (std::ostream&, uri::component);
    std::ostream& operator<< (std::ostream&, uri::span);
}


#endif
