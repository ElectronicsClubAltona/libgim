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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_NOISE_FRACTAL_BASE_IPP
#error
#endif
#define __UTIL_NOISE_FRACTAL_BASE_IPP

#include <cmath>

namespace util { namespace noise { namespace fractal {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename B>
    base<T,B>::base (seed_t _seed,
                     unsigned _octaves,
                     T _H,
                     T _frequency,
                     T _lacunarity,
                     T _amplitude,
                     T _gain):
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
    template <typename T, typename B>
    unsigned
    base<T,B>::octaves (unsigned _octaves)
    {
        return m_octaves = _octaves;
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    constexpr unsigned
    base<T,B>::octaves (void) const
    {
        return m_octaves;
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    T
    base<T,B>::H (T _h)
    {
        m_H = _h;
        m_invAH = std::pow (m_amplitude, -m_H);
        m_invGH = std::pow (m_gain, m_H);
        return m_H;
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    constexpr T
    base<T,B>::H (void) const
    {
        return m_H;
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    T
    base<T,B>::frequency (T _frequency)
    {
        return m_frequency = _frequency;
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    constexpr T
    base<T,B>::frequency (void) const
    {
        return m_frequency;
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    T
    base<T,B>::lacunarity (T _lacunarity)
    {
        return m_lacunarity = _lacunarity;
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    constexpr T
    base<T,B>::lacunarity (void) const
    {
        return m_lacunarity;
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    typename base<T,B>::seed_t
    base<T,B>::seed (seed_t _seed)
    {
        return m_basis.seed (_seed);
    }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    constexpr typename base<T,B>::seed_t
    base<T,B>::seed (void) const
    {
        return m_basis.seed ();
    }
} } }
