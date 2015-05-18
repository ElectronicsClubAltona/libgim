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


#include "noise.hpp"

#include "range.hpp"

#include <iostream>
#include <limits>


//-----------------------------------------------------------------------------
template <typename T>
void
util::noise::fill (image::buffer<T> &pixels,
                   const util::noise::fractal<T> &gen)
{
    size_t h = pixels.h, s = pixels.s, w = pixels.w;
    T *data = pixels.data ();

    for (size_t y = 0; y < h; ++y)
        for (size_t x = 0; x < w; ++x)
            data[y * s + x] = gen (x, y);
}

template void util::noise::fill (image::buffer<float>&, const util::noise::fractal<float>&);
template void util::noise::fill (image::buffer<double>&, const util::noise::fractal<double>&);


//-----------------------------------------------------------------------------
void
util::noise::image2d (uint8_t *restrict pixels,
                      size_t            width,
                      size_t            height,
                      const util::noise::fractal<float> &p) {
    for (size_t y = 0; y < height; ++y)
        for (size_t x = 0; x < width; ++x) {
            double v = p (x, y);
            pixels[x + y * width] = static_cast<uint8_t> (v * std::numeric_limits<uint8_t>::max ());
        }
}
