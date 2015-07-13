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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_RATIONAL_HPP
#define __UTIL_RATIONAL_HPP

#include <type_traits>

namespace util {
    template <typename T>
    struct rational {
        static_assert (std::is_integral<T>::value, "only defined for integer types");

        rational (T n, T d);
        explicit rational (T);
        rational& operator= (const rational&) = default;

        bool operator== (rational) const;
        bool operator!= (rational) const;
        bool operator<  (rational) const;
        bool operator>= (rational) const;

        explicit operator float  (void) const;
        explicit operator double (void) const;

        rational<T> reduced (void) const;

        rational<T>  inverse (void) const;
        rational<T>& invert  (void);

        rational<T> operator+ (T) const;
        rational<T> operator- (T) const;
        rational<T> operator* (T) const;
        rational<T> operator/ (T) const;

        T n;
        T d;
    };

    template <typename T>
    rational<T> operator/ (T, rational<T>);
}

#endif
