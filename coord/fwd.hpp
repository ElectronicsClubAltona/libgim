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

#ifndef CRUFT_UTIL_COORD_FWD_HPP
#define CRUFT_UTIL_COORD_FWD_HPP

#include <cstddef>

namespace util {
    namespace coord {
        template <size_t S,typename T,typename ParentT> struct store;
        template <size_t,typename,typename> struct init;
        template <size_t,typename,typename> struct base;
    }

    template <size_t,typename> struct srgba;
    template <size_t,typename> struct hsva;
    template <size_t,typename> struct extent;
    template <size_t,typename> struct point;
    template <size_t,typename> struct vector;
}

#endif
