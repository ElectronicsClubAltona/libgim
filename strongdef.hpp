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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_STRONGDEF_HPP
#define __UTIL_STRONGDEF_HPP

namespace util {
    /// A transparent wrapper around a (typically lightweight) type for the
    /// purposes of overload disambiguation. It acts like a typesafe typedef.
    template <typename T>
    struct strongdef {
    public:
        using value_type = T;

        constexpr strongdef () = default;
        constexpr explicit strongdef (T _data):
            data (_data)
        { ; }

        strongdef& operator= (const strongdef &rhs)
        { data = rhs.data; return *this; }

        strongdef& operator= (const T &rhs)
        { data = rhs;      return *this; }

        operator const T& (void) const { return data; }
        operator       T& (void)       { return data; }

        bool operator== (const strongdef &rhs) const
        { return data == rhs.data; }

        T data;
    };
}

#endif
