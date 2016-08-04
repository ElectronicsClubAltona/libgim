/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_FIXED_HPP
#define __UTIL_FIXED_HPP

#include "types/bits.hpp"

#include <cstdint>
#include <ostream>

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

            explicit fixed (double);
            explicit fixed (float);
            explicit fixed (native_t);

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
