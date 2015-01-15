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

#ifdef __UTIL_VECTOR_IPP
#error "twice included ipp"
#endif

#define __UTIL_VECTOR_IPP

#include "maths.hpp"

namespace util {
    //-------------------------------------------------------------------------
    template<size_t S, typename T>
    template<size_t D>
    vector<D,T> vector<S,T>::redim (void) const {
        vector<D,T> out;
        std::copy_n (std::begin (this->data),
                     min (S, D),
                     std::begin (out.data));
        return out;
    }


    //-------------------------------------------------------------------------
    template<size_t S, typename T>
    template<size_t D>
    vector<D,T> vector<S,T>::redim (const vector<D,T> &fill) const {
        vector<D,T> out;

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
}
