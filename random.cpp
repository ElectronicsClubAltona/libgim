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


#include "random.hpp"

#include "range.hpp"

#include <type_traits>

using namespace util;

namespace util {
    template <typename T>
    T
    random (void) {
        static_assert (std::is_integral<T>::value, "random should only operate on integral types");
        return range<T>::UNLIMITED.random ();
    }

    template <>
    double
    random (void)
        { return range<double>::UNIT.random (); }

    template <>
    float
    random (void)
        { return range<float>::UNIT.random (); }

    template <typename T>
    T&
    randomise (T &val)
        { return val = util::random<T> (); }
}

template double   util::random (void);
template float    util::random (void);
template uint64_t util::random (void);
template uint32_t util::random (void);
template uint16_t util::random (void);
