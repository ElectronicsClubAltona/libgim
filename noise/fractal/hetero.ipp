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

#ifdef __UTIL_NOISE_FRACTAL_HETERO_IPP
#error
#endif
#define __UTIL_NOISE_FRACTAL_HETERO_IPP

namespace util { namespace noise { namespace fractal {
    //-------------------------------------------------------------------------
    template <typename T, typename B>
    hetero<T,B>::hetero():
        H (0.75f),
        octaves (6),
        frequency (0.1f),
        lacunarity (2),
        offset (0.7f),
        amplitude (1),
        gain (1/lacunarity),
        invAH (std::pow (amplitude, -H)),
        invGH (std::pow (gain, H))
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    constexpr T
    hetero<T,B>::operator() (util::point<2,T> p) const
    {
        T scale = invAH;

        p *= frequency;
        T result = (basis (p) + offset) * scale;
        p *= lacunarity;

        T increment = 0;

        for (size_t i = 1; i < octaves; ++i) {
            scale *= invGH;

            increment  = basis (p) + offset;
            increment *= scale;
            increment *= result;

            result += increment;

            p *= lacunarity;
        }

        return result;
    }
} } }
