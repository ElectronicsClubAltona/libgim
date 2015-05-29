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

#include "perlin.hpp"

#include "../../hash/murmur/murmur2.hpp"

using util::noise::basis::perlin;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::vector<2,T>
generate (util::point<2,intmax_t> p, uint64_t seed)
{
    using util::hash::murmur2::mix;

    auto u = mix (seed, mix (uint64_t (p.x), uint64_t (p.y)));
    auto v = mix (u, seed);

    auto r = util::vector<2,T> {
        (u & 0xffff) / T{0xffff},
        (v & 0xffff) / T{0xffff}
    } * 2 - 1;

    CHECK_GE (r, T{-1});
    CHECK_LE (r, T{ 1});

    return r;
}



///////////////////////////////////////////////////////////////////////////////
template <typename T, util::noise::lerp_t<T> L>
perlin<T,L>::perlin (seed_t _seed):
    seed (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
perlin<T,L>::perlin ():
    seed (time (nullptr))
{ ; }


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
util::range<T>
perlin<T,L>::bounds (void) const
{
    return {
        -std::sqrt (T{2}) / 2,
         std::sqrt (T{2}) / 2
    };
}


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
T
perlin<T,L>::operator() (util::point<2,T> p) const
{
    // extract integer and fractional parts. be careful to always round down
    // (particularly with negatives) and avoid rounding errors.
    auto p_int = p.template cast<intmax_t> ();
    if (p.x < 0) p_int.x -= 1;
    if (p.y < 0) p_int.y -= 1;
    auto p_rem = abs (p - p_int);

    // generate the corner positions
    auto p0 = p_int + util::vector<2,intmax_t> { 0, 0 };
    auto p1 = p_int + util::vector<2,intmax_t> { 1, 0 };
    auto p2 = p_int + util::vector<2,intmax_t> { 0, 1 };
    auto p3 = p_int + util::vector<2,intmax_t> { 1, 1 };

    // generate the corner gradients
    auto g0 = generate<T> (p0, this->seed);
    auto g1 = generate<T> (p1, this->seed);
    auto g2 = generate<T> (p2, this->seed);
    auto g3 = generate<T> (p3, this->seed);

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


///////////////////////////////////////////////////////////////////////////////
#include "../lerp.hpp"

namespace util { namespace noise { namespace basis {
    template struct perlin<float, lerp::linear>;
    template struct perlin<float, lerp::cosine>;
    template struct perlin<float, lerp::cubic>;
    template struct perlin<float, lerp::quintic>;
    template struct perlin<float, lerp::trunc>;

    template struct perlin<double, lerp::linear>;
    template struct perlin<double, lerp::cosine>;
    template struct perlin<double, lerp::cubic>;
    template struct perlin<double, lerp::quintic>;
    template struct perlin<double, lerp::trunc>;
} } }
