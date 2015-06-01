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

#ifdef __UTIL_NOISE_FRACTAL_FBM_IPP
#error
#endif
#define __UTIL_NOISE_FRACTAL_FBM_IPP

#include "../../debug.hpp"

namespace util { namespace noise { namespace fractal {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename B>
    fbm<T,B>::fbm (unsigned _octaves,
                   T        _frequency,
                   T        _lacunarity,
                   T        _amplitude,
                   T        _gain,
                   seed_t   _seed):
        seed (_seed),
        octaves (_octaves),
        frequency (_frequency),
        lacunarity (_lacunarity),
        amplitude (_amplitude),
        gain (_gain),
        basis (_seed)
    {
        CHECK_NEQ (octaves, 0);
        CHECK_NEQ (frequency, 0);
        CHECK_NEQ (amplitude, 0);
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    fbm<T,B>::fbm ():
        fbm<T,B> (DEFAULT_OCTAVES,
                  DEFAULT_FREQUENCY,
                  DEFAULT_LACUNARITY,
                  DEFAULT_AMPLITUDE,
                  DEFAULT_GAIN,
                  rand ())
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    constexpr T
    fbm<T,B>::operator() (util::point<2,T> p) const
    {
        T total = 0;
        T f = frequency;
        T a = amplitude;

        for (size_t i = 0; i < octaves; ++i) {
            total += basis (p * f) * a;

            f *= lacunarity;
            a *= gain;
        }

        return total;
    }
} } }
