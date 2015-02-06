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
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_FIXED_HPP
#define __UTIL_FIXED_HPP

#include "types/bits.hpp"

#include <cstdint>

namespace util {
    template <unsigned I, unsigned E>
    class fixed {
        public:
            typedef typename bits_type<I+E>::uint uint_t;

            fixed (double);
            fixed (float);
            fixed (uint_t);

            double to_double   (void) const;
            float  to_float    (void) const;
            uint_t to_integral (void) const;
            uint_t to_native   (void) const;

            static fixed<I,E> from_native (uint_t);

            static uint_t to_integral (uint_t);

            fixed<I,E>& operator +=(const fixed<I,E>);
            fixed<I,E>& operator -=(const fixed<I,E>);
            fixed<I,E>& operator *=(const fixed<I,E>);
            fixed<I,E>& operator /=(const fixed<I,E>);

            fixed<I,E>  operator +(const fixed<I,E>) const;
            fixed<I,E>  operator -(const fixed<I,E>) const;
            fixed<I,E>  operator *(const fixed<I,E>) const;
            fixed<I,E>  operator /(const fixed<I,E>) const;

            fixed<I,E>& operator +=(uint_t);
            fixed<I,E>& operator -=(uint_t);
            fixed<I,E>& operator *=(uint_t);
            fixed<I,E>& operator /=(uint_t);

            fixed<I,E>  operator +(uint_t) const;
            fixed<I,E>  operator -(uint_t) const;
            fixed<I,E>  operator *(uint_t) const;
            fixed<I,E>  operator /(uint_t) const;

        private:
            uint_t m_value;
    };


    template <unsigned I, unsigned E> bool operator== (util::fixed<I,E>, util::fixed<I,E>);
    template <unsigned I, unsigned E> bool operator!= (util::fixed<I,E>, util::fixed<I,E>);

    template <unsigned I, unsigned E> bool operator<  (util::fixed<I,E>, util::fixed<I,E>);
    template <unsigned I, unsigned E> bool operator<= (util::fixed<I,E>, util::fixed<I,E>);
    template <unsigned I, unsigned E> bool operator>  (util::fixed<I,E>, util::fixed<I,E>);
    template <unsigned I, unsigned E> bool operator>= (util::fixed<I,E>, util::fixed<I,E>);
}

#endif // __UTIL_FIXED_HPP 
