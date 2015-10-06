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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_NOISE_FRACTAL_BASE_IPP
#error
#endif
#define __UTIL_NOISE_FRACTAL_BASE_IPP

#include <cmath>

namespace util { namespace noise { namespace fractal {
    ///////////////////////////////////////////////////////////////////////////
    template <class B>
    base<B>::base (seed_t _seed,
                   unsigned _octaves,
                   value_t _H,
                   value_t _frequency,
                   value_t _lacunarity,
                   value_t _amplitude,
                   value_t _gain):
        // literals
        m_octaves (_octaves),
        m_H (_H),
        m_frequency (_frequency),
        m_lacunarity (_lacunarity),
        m_amplitude (_amplitude),
        m_gain (_gain),
        // compound
        m_basis (_seed),
        // calculated
        m_invAH (std::pow (_amplitude, -_H)),
        m_invGH (std::pow (_gain, _H))
    {
        CHECK_NEQ (m_octaves, 0);
        CHECK_NEQ (m_frequency, 0);
        CHECK_NEQ (m_amplitude, 0);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <class B>
    unsigned
    base<B>::octaves (unsigned _octaves)
    {
        return m_octaves = _octaves;
    }


    //-------------------------------------------------------------------------
    template <class B>
    constexpr unsigned
    base<B>::octaves (void) const
    {
        return m_octaves;
    }


    //-------------------------------------------------------------------------
    template <class B>
    typename base<B>::value_t
    base<B>::H (value_t _h)
    {
        m_H = _h;
        m_invAH = std::pow (m_amplitude, -m_H);
        m_invGH = std::pow (m_gain, m_H);
        return m_H;
    }


    //-------------------------------------------------------------------------
    template <class B>
    constexpr typename base<B>::value_t
    base<B>::H (void) const
    {
        return m_H;
    }


    //-------------------------------------------------------------------------
    template <class B>
    typename base<B>::value_t
    base<B>::frequency (value_t _frequency)
    {
        return m_frequency = _frequency;
    }


    //-------------------------------------------------------------------------
    template <class B>
    constexpr typename base<B>::value_t
    base<B>::frequency (void) const
    {
        return m_frequency;
    }


    //-------------------------------------------------------------------------
    template <class B>
    typename base<B>::value_t
    base<B>::lacunarity (value_t _lacunarity)
    {
        return m_lacunarity = _lacunarity;
    }


    //-------------------------------------------------------------------------
    template <class B>
    constexpr typename base<B>::value_t
    base<B>::lacunarity (void) const
    {
        return m_lacunarity;
    }


    //-------------------------------------------------------------------------
    template <class B>
    constexpr typename base<B>::value_t
    base<B>::amplitude (void) const
    {
        return m_amplitude;
    }


    //-------------------------------------------------------------------------
    template <class B>
    typename base<B>::value_t
    base<B>::amplitude (value_t _amplitude)
    {
        m_amplitude = _amplitude;
        m_invAH = std::pow (m_amplitude, -m_H);
        return m_amplitude;
    }


    //-------------------------------------------------------------------------
    template <class B>
    constexpr typename base<B>::value_t
    base<B>::gain (void) const
    {
        return m_gain;
    }


    //-------------------------------------------------------------------------
    template <class B>
    typename base<B>::value_t
    base<B>::gain (value_t _gain)
    {
        m_gain = _gain;
        m_invGH = std::pow (_gain, m_H);
        return m_gain;
    }


    //-------------------------------------------------------------------------
    template <class B>
    seed_t
    base<B>::seed (seed_t _seed)
    {
        return m_basis.seed (_seed);
    }


    //-------------------------------------------------------------------------
    template <class B>
    seed_t
    base<B>::seed (void) const
    {
        return m_basis.seed ();
    }


    //-------------------------------------------------------------------------
    template <class B>
    const B&
    base<B>::basis (void) const
    {
        return m_basis;
    }


    //-------------------------------------------------------------------------
    template <class B>
    B&
    base<B>::basis (void)
    {
        return m_basis;
    }
} } }
