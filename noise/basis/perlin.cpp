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
generate (util::point<2,T> p, uint64_t seed)
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
    auto p_int = p.template cast<intmax_t> ();
    auto p_rem = p - p_int;

    // Shift the coordinate system down a little to ensure we get unit weights
    // for the lerp. It's better to do this than abs the fractional portion so
    // we don't get reflections along the origin.
    if (p.x < 0) { p_rem.x = 1 + p_rem.x; p_int.x -= 1; }
    if (p.y < 0) { p_rem.y = 1 + p_rem.y; p_int.y -= 1; }

    // Generate the four corner values. It's not strictly necessary to
    // normalise the values, but we get a more consistent and visually
    // appealing range of outputs with normalised values.
    auto p0 = generate<T> (p_int + util::vector<2,T> { 0, 0 }, this->seed).normalise ();
    auto p1 = generate<T> (p_int + util::vector<2,T> { 1, 0 }, this->seed).normalise ();
    auto p2 = generate<T> (p_int + util::vector<2,T> { 0, 1 }, this->seed).normalise ();
    auto p3 = generate<T> (p_int + util::vector<2,T> { 1, 1 }, this->seed).normalise ();

    T v0 = p0.x *  p_rem.x      + p0.y *  p_rem.y;
    T v1 = p1.x * (p_rem.x - 1) + p1.y *  p_rem.y;
    T v2 = p2.x *  p_rem.x      + p2.y * (p_rem.y - 1);
    T v3 = p3.x * (p_rem.x - 1) + p3.y * (p_rem.y - 1);

    auto L0 = L (v0, v1, p_rem.x);
    auto L1 = L (v2, v3, p_rem.x);
    auto L_ = L (L0, L1, p_rem.y);
    return L_;
}


///////////////////////////////////////////////////////////////////////////////
#include "../lerp.hpp"

namespace util { namespace noise { namespace basis {
    template struct perlin<float, lerp::linear>;
    template struct perlin<float, lerp::cubic>;
    template struct perlin<float, lerp::quintic>;

    template struct perlin<double, lerp::linear>;
    template struct perlin<double, lerp::cubic>;
    template struct perlin<double, lerp::quintic>;
} } }
