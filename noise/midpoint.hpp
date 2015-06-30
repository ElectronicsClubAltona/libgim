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

#ifndef __UTIL_NOISE_MIDPOINT_HPP
#define __UTIL_NOISE_MIDPOINT_HPP

#include "image.hpp"

namespace util { namespace noise {
    template <typename T>
    void
    midpoint (image::buffer<T>&, uint64_t seed, float persistence = 0.65f, float sides = 0.25f);
} }

#include "midpoint.ipp"

#endif
