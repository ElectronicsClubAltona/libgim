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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_NOISE_LUT_HPP
#define __UTIL_NOISE_LUT_HPP

#include <cstdlib>
#include <type_traits>

#include "types.hpp"

namespace util {
    namespace noise {
        extern const size_t PERM[256];
        extern const double LUT [256];

        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, size_t>::type
        permute (T idx) {
            return PERM[(size_t)idx % elems (PERM)];
        }

        template <typename T1, typename T2, typename ...R>
        size_t permute (T1 t1, T2 t2, R ...r) {
            auto p  = permute (t1);
                 p += permute (t2, r...);
            return p % elems (PERM);
        }
    }
}

#endif
