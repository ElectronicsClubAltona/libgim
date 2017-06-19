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

#ifndef __UTIL_TYPES_COMPARATOR_HPP
#define __UTIL_TYPES_COMPARATOR_HPP

#include <memory>

#include "./iterator.hpp"

namespace util::comparator {
    template <typename T>
    struct pointer_comparator {
        bool operator() (const std::unique_ptr<T> &lhs, const std::unique_ptr<T> &rhs);
        bool operator() (const T                  *lhs, const std::unique_ptr<T> &rhs);
        bool operator() (const std::unique_ptr<T> &lhs, const T                  *rhs);
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

#include "./comparator.ipp"
#endif
