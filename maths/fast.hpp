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

#ifndef __UTIL_MATHS_FAST_HPP
#define __UTIL_MATHS_FAST_HPP

namespace util { namespace maths { namespace fast {
    ///////////////////////////////////////////////////////////////////////////
    constexpr float pow (float, float);

    constexpr float pow2 (float);


    ///////////////////////////////////////////////////////////////////////////
    constexpr float exp (float);


    ///////////////////////////////////////////////////////////////////////////
    constexpr float log (float);

    constexpr float log2 (float);


    ///////////////////////////////////////////////////////////////////////////
    constexpr float sqrt (float);
    constexpr float invsqrt (float);
} } }

#include "fast.ipp"

#endif
