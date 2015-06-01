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

#include "../../hash/murmur/murmur2.hpp"

namespace util { namespace noise { namespace basis {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, util::noise::lerp_t<T> L>
    value<T,L>::value (seed_t _seed):
        seed (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, util::noise::lerp_t<T> L>
    value<T,L>::value ():
        seed (time (nullptr))
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, util::noise::lerp_t<T> L>
    util::range<T>
    value<T,L>::bounds (void) const
    {
        return { -1, 1 };
    }


    //-------------------------------------------------------------------------
    template <typename T, util::noise::lerp_t<T> L>
    constexpr T
    value<T,L>::operator() (util::point<2,T> p) const
    {
        // extract integer and fractional parts. be careful to always round down
        // (particularly with negatives) and avoid rounding errors.
        auto p_int = p.template cast<intmax_t> ();
        if (p.x < 0) p_int.x -= 1;
        if (p.y < 0) p_int.y -= 1;
        auto p_rem = abs (p - p_int);

        // generate the corner points
        auto p0 = p_int + util::vector<2,intmax_t> { 0, 0 };
        auto p1 = p_int + util::vector<2,intmax_t> { 1, 0 };
        auto p2 = p_int + util::vector<2,intmax_t> { 0, 1 };
        auto p3 = p_int + util::vector<2,intmax_t> { 1, 1 };

        // Generate the four corner values
        T g0 = generate (p0);
        T g1 = generate (p1);
        T g2 = generate (p2);
        T g3 = generate (p3);

        // Interpolate on one dimension, then the other.
        auto l0 = L (g0, g1, p_rem.x);
        auto l1 = L (g2, g3, p_rem.x);
        auto l_ = L (l0, l1, p_rem.y);

        return l_;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Generate a type from [-UNIT..UNIT]
    template <typename T, lerp_t<T> L>
    constexpr T
    value<T,L>::generate (point<2,intmax_t> p) const
    {
        using util::hash::murmur2::mix;

        T v = mix (
            seed,
            mix (
                uint64_t (p.y),
                uint64_t (p.x)
            )
        ) & 0xffff;

        v = v / T{0xffff} * 2 - 1;
        return v;
    }
} } }
