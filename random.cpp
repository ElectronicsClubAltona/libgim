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


#include "random.hpp"

#include "range.hpp"

using namespace util;

namespace util {
    template <typename T>
    T
    random (void)
        { return range<T>::UNLIMITED.rand (); }

    template <typename T>
    T&
    randomise (T &val)
        { return val = util::random<T> (); }
}

template double   util::random (void);
template float    util::random (void);
template uint64_t util::random (void);

