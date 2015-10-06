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
    template <class B>
    rmf<B>::rmf (seed_t _seed,
                 unsigned _octaves,
                 value_t  _H,
                 value_t  _offset,
                 value_t  _frequency,
                 value_t  _lacunarity,
                 value_t  _amplitude,
                 value_t  _gain):
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
    rmf<B>::rmf (seed_t _seed):
        rmf<B> (_seed,
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
    template <class B>
    typename rmf<B>::value_t
    rmf<B>::operator() (point_t p) const
    {
        value_t scale = this->m_invAH;

        value_t signal = 0;
        value_t result = 0;
        value_t weight = 1;

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

            p += PI<value_t>;
            p *= this->m_lacunarity;
        }

        return result;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <class B>
    typename rmf<B>::value_t
    rmf<B>::offset (void) const
    {
        return m_offset;
    }


    //-------------------------------------------------------------------------
    template <class B>
    typename rmf<B>::value_t
    rmf<B>::offset (value_t _offset)
    {
        return m_offset = _offset;
    }
} } }
