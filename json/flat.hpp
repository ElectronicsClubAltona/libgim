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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_JSON_FLAT_HPP
#define __UTIL_JSON_FLAT_HPP

#include <ostream>
#include <vector>

#include "../view.hpp"


///////////////////////////////////////////////////////////////////////////////
namespace json::flat {
    enum class type {
        UNKNOWN,

        NUL,
        BOOLEAN,
        STRING,
        INTEGER,
        REAL,

        OBJECT_BEGIN,
        OBJECT_END,

        ARRAY_BEGIN,
        ARRAY_END
    };

    template <typename T>
    struct item {
        type tag;
        T first;
        T last;

        template <typename U>
        U as (void) const;
    };

    template <typename T>
    std::vector<item<T>>
    parse (util::view<T> data);

    std::ostream& operator<< (std::ostream&, type);
}

#endif
