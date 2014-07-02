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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_RATIONAL_HPP
#define __UTIL_RATIONAL_HPP

namespace util {
    template <typename T>
    struct rational {
        rational (T _n, T _d);
        rational& operator=  (const rational&) = default;

        bool operator== (const rational&);

        explicit operator float  (void) const;
        explicit operator double (void) const;

        T n;
        T d;
    };
}

#endif
