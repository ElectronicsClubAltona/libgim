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

#ifdef __UTIL_NOISE_BASIS_PERLIN_IPP
#error
#endif
#define __UTIL_NOISE_BASIS_PERLIN_IPP

#include "../rand.hpp"

namespace util { namespace noise { namespace basis {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, util::noise::lerp_t<T> L, template <typename,lerp_t<T>> class G>
    perlin<T,L,G>::perlin (seed_t _seed):
        G<T,L> (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, util::noise::lerp_t<T> L, template <typename,lerp_t<T>> class G>
    util::range<T>
    perlin<T,L,G>::bounds (void) const
    {
        return {
            -std::sqrt (T{2}) / 2,
             std::sqrt (T{2}) / 2
        };
    }


    //-------------------------------------------------------------------------
    template <typename T, util::noise::lerp_t<T> L, template <typename,lerp_t<T>> class G>
    T
    perlin<T,L,G>::operator() (util::point<2,T> p) const
    {
        // extract integer and fractional parts. be careful to always round down
        // (particularly with negatives) and avoid rounding errors.
        auto p_int = p.template cast<intmax_t> ();
        if (p.x < 0) p_int.x -= 1;
        if (p.y < 0) p_int.y -= 1;
        auto p_rem = p - p_int;

        // generate the corner positions
        auto p0 = p_int + util::vector<2,intmax_t> { 0, 0 };
        auto p1 = p_int + util::vector<2,intmax_t> { 1, 0 };
        auto p2 = p_int + util::vector<2,intmax_t> { 0, 1 };
        auto p3 = p_int + util::vector<2,intmax_t> { 1, 1 };

        // generate the corner gradients
        auto g0 = G<T,L>::generate (p0);
        auto g1 = G<T,L>::generate (p1);
        auto g2 = G<T,L>::generate (p2);
        auto g3 = G<T,L>::generate (p3);

        // compute the dot products
        T v0 = dot (g0, p - p0);
        T v1 = dot (g1, p - p1);
        T v2 = dot (g2, p - p2);
        T v3 = dot (g3, p - p3);

        // interpolate the results
        auto L0 = L (v0, v1, p_rem.x);
        auto L1 = L (v2, v3, p_rem.x);
        auto L_ = L (L0, L1, p_rem.y);

        return L_;
    }
} } }
