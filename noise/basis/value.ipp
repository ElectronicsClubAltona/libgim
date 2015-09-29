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

#ifdef __UTIL_NOISE_BASIS_VALUE_IPP
#error
#endif
#define __UTIL_NOISE_BASIS_VALIE_IPP

#include "../rand.hpp"

namespace util { namespace noise { namespace basis {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, util::noise::lerp_t<T> L>
    value<T,L>::value (seed_t _seed):
        m_seed (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, util::noise::lerp_t<T> L>
    util::range<T>
    value<T,L>::bounds (void) const
    {
        return { -1, 1 };
    }


    //-------------------------------------------------------------------------
    template <typename T, lerp_t<T> L>
    seed_t
    value<T,L>::seed (void) const
    {
        return m_seed;
    }


    //-------------------------------------------------------------------------
    template <typename T, lerp_t<T> L>
    seed_t
    value<T,L>::seed (seed_t _seed)
    {
        return m_seed = _seed;
    }


    //-------------------------------------------------------------------------
    template <typename T, util::noise::lerp_t<T> L>
    T
    value<T,L>::operator() (util::point<2,T> p) const
    {
        // extract integer and fractional parts. be careful to always round down
        auto p_int = floor (p).template cast<intmax_t> ();
        auto p_rem = p - p_int;

        // generate the corner points
        auto p0 = p_int + util::vector<2,intmax_t> { 0, 0 };
        auto p1 = p_int + util::vector<2,intmax_t> { 1, 0 };
        auto p2 = p_int + util::vector<2,intmax_t> { 0, 1 };
        auto p3 = p_int + util::vector<2,intmax_t> { 1, 1 };

        // Generate the four corner values
        T g0 = noise::rand<float> (m_seed, p0);
        T g1 = noise::rand<float> (m_seed, p1);
        T g2 = noise::rand<float> (m_seed, p2);
        T g3 = noise::rand<float> (m_seed, p3);

        // Interpolate on one dimension, then the other.
        auto l0 = L (g0, g1, p_rem.x);
        auto l1 = L (g2, g3, p_rem.x);
        auto l_ = L (l0, l1, p_rem.y);

        return l_;
    }
} } }
