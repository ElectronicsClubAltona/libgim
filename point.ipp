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

#include "maths.hpp"

#include <algorithm>

namespace util {
    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    template <typename U>
    typename std::common_type<T,U>::type
    util::point<S,T>::distance (const point<S,U> &rhs) const {
        return std::sqrt (distance2 (rhs));
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    template <typename U>
    typename std::common_type<T,U>::type
    util::point<S,T>::distance2 (const point<S,U> &rhs) const {
        typedef typename std::common_type<T,U>::type result_t;

        result_t sum { 0 };

        for (size_t i = 0; i < S; ++i)
            sum += pow2 (this->data[i] - rhs.data[i]);

        return sum;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    template <typename U>
    typename std::common_type<T,U>::type
    util::point<S,T>::manhattan (const point<S,U> &rhs) const {
        typedef typename std::common_type<T,U>::type result_t;

        result_t sum { 0 };

        for (size_t i = 0; i < S; ++i)
            sum += std::abs (this->data[i] - rhs.data[i]);

        return sum;
    }


    //-------------------------------------------------------------------------
    template<size_t S, typename T>
    template<size_t D>
    point<D,T> point<S,T>::redim (void) const {
        point<D,T> out;
        std::copy (std::begin (this->data), std::begin (this->data) + D, std::begin (out.data));
        return out;
    }
}
