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

#ifndef __UTIL_NOISE_BASIS_TYPE_GRADIENT_HPP
#define __UTIL_NOISE_BASIS_TYPE_GRADIENT_HPP

#include "../../../point.hpp"
#include "../../../maths.hpp"

#include <array>


///////////////////////////////////////////////////////////////////////////////
namespace util { namespace noise { namespace basis { namespace type {
    template <size_t S>
    struct gradient {

    protected:
        static const std::array<vectori<S>,util::pow(2,S)> CORNERS;
    };
} } } }

#endif
