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

#ifndef __UTIL_STATS_HPP
#define __UTIL_STATS_HPP

#include <iostream>


namespace util {
    namespace stats {
        // Undefined until at least one value has been added.

        template <typename T>
        struct accumulator {
            accumulator ();

            void add (T);
            void add (const accumulator<T> &);

            size_t count;

            T min;
            T max;
            T sum;

            T range (void) const;
            T mean  (void) const;
        };

        template <typename T>
        std::ostream& operator<< (std::ostream&, const accumulator<T>&);
    }
}

#endif
