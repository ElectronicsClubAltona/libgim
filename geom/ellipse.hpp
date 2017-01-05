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

#ifndef __UTIL_GEOM_ELLIPSE_HPP
#define __UTIL_GEOM_ELLIPSE_HPP

#include <cstdlib>

#include "../point.hpp"
#include "../vector.hpp"

namespace util::geom {
    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    struct ellipse {
        util::point<S,T> origin;
        util::vector<S,T> radius;
    };
}

#include "ellipse.ipp"

#endif
