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

#ifndef __UTIL_NOISE_LUT_HPP
#define __UTIL_NOISE_LUT_HPP

#include <cstdlib>
#include <type_traits>

#include "types.hpp"

namespace util {
    namespace noise {
        extern const size_t PERM[256];
        extern const double LUT [256];

        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, size_t>::type
        permute (T idx) {
            return PERM[(size_t)idx % elems (PERM)];
        }

        template <typename T1, typename T2, typename ...R>
        size_t permute (T1 t1, T2 t2, R ...r) {
            auto p  = permute (t1);
                 p += permute (t2, r...);
            return p % elems (PERM);
        }
    }
}

#endif
