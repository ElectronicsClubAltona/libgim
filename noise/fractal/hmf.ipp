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

#ifdef __UTIL_NOISE_FRACTAL_HMF_IPP
#error
#endif
#define __UTIL_NOISE_FRACTAL_HMF_IPP


namespace util { namespace noise { namespace fractal {
    ///////////////////////////////////////////////////////////////////////////
    template <class B>
    hmf<B>::hmf (seed_t _seed,
                 unsigned _octaves,
                 value_t _H,
                 value_t _frequency,
                 value_t _lacunarity,
                 value_t _amplitude,
                 value_t _gain,
                 value_t _offset):
        base<B> (_seed,
                 _octaves,
                 _H,
                 _frequency,
                 _lacunarity,
                 _amplitude,
                 _gain),
        m_offset (_offset)
    { ; }


    //-------------------------------------------------------------------------
    template <class B>
    hmf<B>::hmf (seed_t _seed):
        hmf<B> (_seed,
                DEFAULT_OCTAVES,
                DEFAULT_H,
                DEFAULT_FREQUENCY,
                DEFAULT_LACUNARITY,
                DEFAULT_AMPLITUDE,
                DEFAULT_GAIN,
                DEFAULT_OFFSET)
    { ; }


    ///////////////////////////////////////////////////////////////////////////
    template <class B>
    typename hmf<B>::value_t
    hmf<B>::operator() (point_t p) const
    {
        value_t scale = this->m_invAH;

        value_t result = 0;
        value_t signal = 0;
        value_t weight = 1;

        p *= this->m_frequency;

        for (size_t i = 0; i < this->m_octaves; ++i) {
            signal  = (this->m_basis (p) + m_offset) * scale;
            result += signal * weight;

            weight *= signal;
            weight  = min (weight, value_t{1});

            scale *= this->m_invGH;
            p += PI<value_t>;
            p *= this->m_lacunarity;
        }

        return result;
    }
} } }
