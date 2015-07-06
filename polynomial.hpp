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

#ifndef __UTIL_POLYNOMIAL_HPP
#define __UTIL_POLYNOMIAL_HPP

#include <array>
#include <cstdlib>

namespace util {
    namespace polynomial {
        // Invalid solutions are represented by NaN. They are guaranteed to
        // be at the end of the solution list, so they are safe to skip on the
        // first instance.
        template <size_t S>
        std::array<float,S>
        roots (std::array<float,S+1>);

        template <size_t S, typename T, typename U>
        T
        eval (std::array<T,S>, U x);
    }
}

#include "polynomial.ipp"

#endif
