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

#ifdef __UTIL_NOISE_FRACTAL_RMF_IPP
#error
#endif
#define __UTIL_NOISE_FRACTAL_RMF_IPP


namespace util { namespace noise { namespace fractal {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename B>
    rmf<T,B>::rmf (seed_t _seed,
                   unsigned _octaves,
                   T        _H,
                   T        _offset,
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
                   _gain),
        m_offset (_offset)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    rmf<T,B>::rmf (seed_t _seed):
        rmf<T,B> (_seed,
                  DEFAULT_OCTAVES,
                  DEFAULT_H,
                  DEFAULT_FREQUENCY,
                  DEFAULT_LACUNARITY,
                  DEFAULT_AMPLITUDE,
                  DEFAULT_GAIN,
                  DEFAULT_OFFSET)
    { ; }


    ///////////////////////////////////////////////////////////////////////////
    // we use the name 'amplitude' instead of musgrave's 'gain'.
    // assumes basis distribution [-1,1] and offset ~= 1
    template <typename T, typename B>
    constexpr T
    rmf<T,B>::operator() (util::point<2,T> p) const
    {
        T scale = this->m_invAH;

        T signal = 0;
        T result = 0;
        T weight = 1;

        p *= this->m_frequency;

        for (size_t i = 0; i < this->m_octaves; ++i) {
            // generates ridged noise
            signal = this->m_basis (p);
            signal = std::fabs (signal);
            signal = m_offset - signal;

            // sharpens the ridges
            signal *= signal;

            // influence by sharpness of previous iteration
            signal *= weight;

            // contribute to the weight
            weight = signal * this->m_amplitude;
            weight = limit (weight, 0, 1);

            // record and continue
            result += signal * scale;

            scale *= this->m_invGH;
            p *= this->m_lacunarity;
        }

        return result;
    }
} } }
