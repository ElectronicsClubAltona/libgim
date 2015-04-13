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

namespace util {
    template <typename T>
    struct rational {
        rational (T _n, T _d);
        rational& operator=  (const rational&) = default;

        bool operator== (const rational&);

        explicit operator float  (void) const;
        explicit operator double (void) const;
        explicit operator int    (void) const;

        rational<T>  inverse (void) const;
        rational<T>& invert  (void);

        template <typename U> rational<T> operator* (const U&) const;
        template <typename U> rational<T> operator/ (const U&) const;

        T n;
        T d;
    };

    template <typename T, typename U>
    rational<T> operator/ (U, rational<T>);
}

#endif
