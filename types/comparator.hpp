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
 * Copyright 2012-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef UTIL_TYPES_COMPARATOR_HPP
#define UTIL_TYPES_COMPARATOR_HPP

#include <memory>

#include "./iterator.hpp"

namespace util::comparator {
    ///////////////////////////////////////////////////////////////////////////
    /// Compares standard types that can be reduced to pointers.
    template <typename T>
    struct pointer {
        constexpr
        bool
        operator() (const std::unique_ptr<T> &a, const std::unique_ptr<T> &b)
        {
            return a < b;
        }

        constexpr
        bool
        operator() (const T *a, const std::unique_ptr<T> &b)
        {
            return a < b.get ();
        }

        constexpr
        bool
        operator() (const std::unique_ptr<T> &a, const T *b)
        {
            return a.get () < b;
        }
    };


    ///////////////////////////////////////////////////////////////////////////
    /// Provides a comparison interface for types exposing an index operator.
    ///
    /// Will return true if the first parameter has the first index that
    /// compares less than the second parameter. Useful for providing a total
    /// order of coordinate types where this doesn't make sense by default.
    ///
    /// \tparam T The container data type to compare. Must provide an index
    /// operator.
    template <typename T>
    struct indexed {
        constexpr
        bool operator() (const T &a, const T &b)
        {
            for (auto x: util::zip (a, b)) {
                const auto &[i,j] = x; // clang-4.0: workaround for segfault.
                if (i  < j) return true;
                if (i != j) return false;
            }

            return false;
        }
    };
}

#endif
