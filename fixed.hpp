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
#include <iostream>

namespace util {
    template <typename T, unsigned I, unsigned E>
    class fixed {
        public:
            typedef typename std::conditional<
                std::is_signed<T>::value,
                typename bits_type<I+E>::sint,
                typename bits_type<I+E>::uint
            >::type native_t;

            typedef native_t integer_t;

            fixed (double);
            fixed (float);
            fixed (native_t);

            double    to_double  (void) const;
            float     to_float   (void) const;
            integer_t to_integer (void) const;
            native_t  to_native  (void) const;

            static fixed<T,I,E> from_native (native_t);
            static integer_t to_integer (native_t);

            fixed<T,I,E>& operator +=(const fixed<T,I,E>);
            fixed<T,I,E>& operator -=(const fixed<T,I,E>);
            fixed<T,I,E>& operator *=(const fixed<T,I,E>);
            fixed<T,I,E>& operator /=(const fixed<T,I,E>);

            fixed<T,I,E>  operator +(const fixed<T,I,E>) const;
            fixed<T,I,E>  operator -(const fixed<T,I,E>) const;
            fixed<T,I,E>  operator *(const fixed<T,I,E>) const;
            fixed<T,I,E>  operator /(const fixed<T,I,E>) const;

            fixed<T,I,E>& operator +=(integer_t);
            fixed<T,I,E>& operator -=(integer_t);
            fixed<T,I,E>& operator *=(integer_t);
            fixed<T,I,E>& operator /=(integer_t);

            fixed<T,I,E>  operator +(integer_t) const;
            fixed<T,I,E>  operator -(integer_t) const;
            fixed<T,I,E>  operator *(integer_t) const;
            fixed<T,I,E>  operator /(integer_t) const;

        private:
            fixed () = default;

            native_t m_value;
    };


    template <typename T, unsigned I, unsigned E> bool operator== (util::fixed<T,I,E>, util::fixed<T,I,E>);
    template <typename T, unsigned I, unsigned E> bool operator!= (util::fixed<T,I,E>, util::fixed<T,I,E>);

    template <typename T, unsigned I, unsigned E> bool operator<  (util::fixed<T,I,E>, util::fixed<T,I,E>);
    template <typename T, unsigned I, unsigned E> bool operator<= (util::fixed<T,I,E>, util::fixed<T,I,E>);
    template <typename T, unsigned I, unsigned E> bool operator>  (util::fixed<T,I,E>, util::fixed<T,I,E>);
    template <typename T, unsigned I, unsigned E> bool operator>= (util::fixed<T,I,E>, util::fixed<T,I,E>);


    template <typename T, unsigned I, unsigned E>
    std::ostream& operator<< (std::ostream&, fixed<T,I,E>);
}

#endif // __UTIL_FIXED_HPP
