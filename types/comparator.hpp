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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_TYPES_COMPARATOR_HPP
#define __UTIL_TYPES_COMPARATOR_HPP

#include <memory>

namespace util {
    template <typename T>
    struct pointer_comparator {
        bool operator() (const std::unique_ptr<T> &lhs, const std::unique_ptr<T> &rhs);
        bool operator() (const T                  *lhs, const std::unique_ptr<T> &rhs);
        bool operator() (const std::unique_ptr<T> &lhs, const T                  *rhs);
    };
}

#include "util/types/comparator.ipp"
#endif
