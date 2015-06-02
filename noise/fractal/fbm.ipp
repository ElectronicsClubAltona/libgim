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
    fbm<T,B>::fbm (seed_t _seed,
                   unsigned _octaves,
                   T        _H,
                   T        _frequency,
                   T        _lacunarity,
                   T        _amplitude,
                   T        _gain):
        base<T,B> (_seed,
                   _octaves,
                   _H,
                   _frequency,
                   _lacunarity,
                   _amplitude,
                   _gain)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    fbm<T,B>::fbm (seed_t _seed):
        fbm<T,B> (_seed,
                  DEFAULT_OCTAVES,
                  DEFAULT_H,
                  DEFAULT_FREQUENCY,
                  DEFAULT_LACUNARITY,
                  DEFAULT_AMPLITUDE,
                  DEFAULT_GAIN)
    { ; }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename B>
    constexpr T
    fbm<T,B>::operator() (util::point<2,T> p) const
    {
        T total = 0;
        T scale = this->m_invAH;

        p *= this->m_frequency;

        for (size_t i = 0; i < this->m_octaves; ++i) {
            total += this->m_basis (p) * scale;

            p *= this->m_lacunarity;
            scale *= this->m_invGH;
        }

        return total;
    }
} } }
