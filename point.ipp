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
 * Copyright 2014-2015 Danny Robson <danny@nerdcruft.net>
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
        std::copy_n (std::begin (this->data),
                     min (S, D),
                     std::begin (out.data));
        return out;
    }


    //-------------------------------------------------------------------------
    template<size_t S, typename T>
    template<size_t D>
    point<D,T> point<S,T>::redim (const point<D,T> &fill) const {
        point<D,T> out;

        static constexpr auto L1 = min (S, D);
        static constexpr auto L2 = D - L1;

        std::copy_n (std::begin (this->data),
                     L1,
                     std::begin (out.data));

        std::copy_n (fill.data + L1,
                     L2,
                     out.data + L1);
        return out;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    template <size_t D>
    point<D,T> point<S,T>::redim (T fill) const
    {
        point<D,T> out;

        auto cursor = std::copy_n (std::begin (this->data),
                                   min (S, D),
                                   std::begin (out.data));
        std::fill (cursor, std::end (out.data), fill);

        return out;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    template <typename U>
    point<S,U>
    point<S,T>::cast (void) const
    {
        point<S,U> out;
        std::copy (std::begin (this->data),
                   std::end   (this->data),
                   std::begin (out.data));
        return out;
    }
}
