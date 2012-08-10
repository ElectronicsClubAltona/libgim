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

#ifndef __UTIL_MATHS_POLYNOMIAL_HPP
#define __UTIL_MATHS_POLYNOMIAL_HPP

#include <array>
#include <cstddef>

namespace maths {
    template <typename T, size_t N>
    struct polynomial {
        std::array<T,N> coefficients;

        T eval (T x) const;
    };
}

#include "polynomial.ipp"

#endif
