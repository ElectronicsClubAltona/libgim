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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_PERLIN_HPP
#define __UTIL_PERLIN_HPP

#include <cstdint>
#include <cstdlib>

#include "noise/basis.hpp"
#include "noise/fractal.hpp"

#include "image.hpp"

namespace util {
    namespace noise {
        void fill (image::buffer<float>&, const util::noise::fractal&);

        void image2d (uint8_t *restrict pixels, size_t width, size_t height, const util::noise::fractal&);
    }
}


#endif
