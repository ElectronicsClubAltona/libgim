/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUvalue_t WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
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
    ///////////////////////////////////////////////////////////////////////////
    template <class B>
    hetero<B>::hetero(seed_t _seed,
                      unsigned _octaves,
                      value_t _H,
                      value_t _frequency,
                      value_t _lacunarity,
                      value_t _amplitude,
                      value_t _gain):
        hetero<B> (_seed,
                   _octaves,
                   _H,
                   _frequency,
                   _lacunarity,
                   _amplitude,
                   _gain,
                   -this->basis ().bounds ().min + this->basis ().bounds ().magnitude () / value_t{2})
    { ; }


    //-------------------------------------------------------------------------
    template <class B>
    hetero<B>::hetero(seed_t _seed,
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
    hetero<B>::hetero (seed_t _seed):
        hetero<B> (_seed,
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
    constexpr typename hetero<B>::value_t
    hetero<B>::offset (void) const
    {
        return m_offset;
    }


    //-------------------------------------------------------------------------
    template <class B>
    typename hetero<B>::value_t
    hetero<B>::offset (value_t _offset)
    {
        return m_offset = _offset;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <class B>
    typename hetero<B>::value_t
    hetero<B>::operator() (point_t p) const
    {
        value_t scale = this->m_invAH;

        p *= this->m_frequency;
        value_t result = (this->m_basis (p) + m_offset) * scale;
        p *= this->m_lacunarity;

        value_t increment = 0;

        for (size_t i = 1; i < this->m_octaves; ++i) {
            scale *= this->m_invGH;

            increment  = this->m_basis (p) + m_offset;
            increment *= scale;
            increment *= result;

            result += increment;

            p += PI<value_t>;
            p *= this->m_lacunarity;
        }

        return result;
    }
} } }
