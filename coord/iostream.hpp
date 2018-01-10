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
 * Copyright 2016-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_IOSTREAM
#define CRUFT_UTIL_IOSTREAM

#include "./traits.hpp"
#include "../iterator.hpp"

#include <cstddef>
#include <ostream>
#include <algorithm>

namespace util {
    template <
        typename K,
        typename = std::enable_if_t<is_coord_v<K>,void>
    >
    std::ostream&
    operator<< (std::ostream &os, const K &k)
    {
        os << "[";
        std::transform (std::cbegin (k),
                        std::cend   (k),
                        infix_iterator<typename K::value_type> (os, ", "),
                        [] (auto i) { return +i; });
        os << "]";

        return os;
    }
}

#endif
