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

#ifdef __UTIL_NOISE_IPP
#error
#endif
#define __UTIL_NOISE_IPP

namespace util { namespace noise {
    //-------------------------------------------------------------------------
    template <typename T, typename G>
    void
    fill (image::buffer<T> &pixels, const G& gen)
    {
        size_t h = pixels.h, s = pixels.s, w = pixels.w;
        T *data = pixels.data ();

        for (size_t y = 0; y < h; ++y)
            for (size_t x = 0; x < w; ++x)
                data[y * s + x] = gen ({T(x), T(y)});
    }
} }
