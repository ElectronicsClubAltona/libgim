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
    template <size_t S, typename T, typename B>
    hmf<S,T,B>::hmf (seed_t _seed,
                     unsigned _octaves,
                     T _H,
                     T _frequency,
                     T _lacunarity,
                     T _amplitude,
                     T _gain,
                     T _offset):
        base<S,T,B> (_seed,
                     _octaves,
                     _H,
                     _frequency,
                     _lacunarity,
                     _amplitude,
                     _gain),
        m_offset (_offset)
    { ; }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, typename B>
    hmf<S,T,B>::hmf (seed_t _seed):
        hmf<S,T,B> (_seed,
                    DEFAULT_OCTAVES,
                    DEFAULT_H,
                    DEFAULT_FREQUENCY,
                    DEFAULT_LACUNARITY,
                    DEFAULT_AMPLITUDE,
                    DEFAULT_GAIN,
                    DEFAULT_OFFSET)
    { ; }


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T, typename B>
    T
    hmf<S,T,B>::operator() (point<S,T> p) const
    {
        T scale = this->m_invAH;

        T result = 0;
        T signal = 0;
        T weight = 1;

        p *= this->m_frequency;

        for (size_t i = 0; i < this->m_octaves; ++i) {
            signal  = (this->m_basis (p) + m_offset) * scale;
            result += signal * weight;

            weight *= signal;
            weight  = min (weight, T{1});

            scale *= this->m_invGH;
            p += PI<T>;
            p *= this->m_lacunarity;
        }

        return result;
    }
} } }
