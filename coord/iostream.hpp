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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_IOSTREAM
#define __UTIL_IOSTREAM

#include "../iterator.hpp"
#include "../stream.hpp"

#include <ostream>
#include <algorithm>

namespace util {
    template <
        template <size_t,typename> class K,
        size_t S,
        typename T
    >
    std::ostream&
    operator<< (std::ostream &os, const K<S,T> &k)
    {
        os << "[";
        std::transform (std::cbegin (k),
                        std::cend   (k),
                        infix_iterator<
                            stream::numeric<T>
                        > (os, ", "),
                        stream::to_numeric<T>);
        os << "]";

        return os;
    }

}

#endif
