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
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_NOISE_BASIS_CONSTANT_HPP
#define __UTIL_NOISE_BASIS_CONSTANT_HPP

#include "../../point.hpp"

#include <cstdint>

namespace util { namespace noise { namespace basis {
    template <typename T>
    struct constant {
        using seed_t = uint64_t;

        constant (seed_t);
        constexpr T operator() (util::point<2,T>) const;

        seed_t seed;
        T value;
    };
} } }


#include "constant.ipp"

#endif
