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

#include "value.hpp"

#include "../../hash/murmur/murmur2.hpp"

using util::noise::basis::value;


///////////////////////////////////////////////////////////////////////////////
// Generate a type from [-UNIT..UNIT]
template <typename T>
T
generate (util::point<2,T> p, uint64_t seed)
{
    using util::hash::murmur2::mix;

    T v = mix (seed, mix (uint64_t (p.y), uint64_t (p.x))) & 0xffff;
    v = v / T{0xffff} * 2 - 1;

    CHECK_GE (v, T{0});
    CHECK_LE (v, T{1});

    return v;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T, util::noise::lerp_t<T> L>
value<T,L>::value (seed_t _seed):
    seed (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
value<T,L>::value ():
    seed (time (nullptr))
{ ; }


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
util::range<T>
value<T,L>::bounds (void) const
{
    return { -1, 1 };
}


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
T
value<T,L>::operator() (util::point<2,T> p) const
{
    auto p_int = p.template cast<intmax_t> ();
    auto p_rem = p - p_int;

    // Shift the coordinate system down a little to ensure we get unit weights
    // for the lerp. It's better to do this than abs the fractional portion so
    // we don't get reflections along the origin.
    if (p.x < 0) { p_rem.x = 1 + p_rem.x; p_int.x -= 1; }
    if (p.y < 0) { p_rem.y = 1 + p_rem.y; p_int.y -= 1; }

    // Generate the four corner values
    T p0 = generate<T> (p_int + util::vector<2,T>{ 0, 0 }, this->seed);
    T p1 = generate<T> (p_int + util::vector<2,T>{ 1, 0 }, this->seed);
    T p2 = generate<T> (p_int + util::vector<2,T>{ 0, 0 }, this->seed);
    T p3 = generate<T> (p_int + util::vector<2,T>{ 1, 1 }, this->seed);

    // Interpolate on one dimension, then the other.
    return L (L (p0, p1, p_rem.x),
              L (p2, p3, p_rem.x),
              p_rem.y);
}


///////////////////////////////////////////////////////////////////////////////
#include "../lerp.hpp"

namespace util { namespace noise { namespace basis {
    template struct value<float, lerp::trunc>;
    template struct value<float, lerp::linear>;
    template struct value<float, lerp::cubic>;
    template struct value<float, lerp::quintic>;

    template struct value<double, lerp::trunc>;
    template struct value<double, lerp::linear>;
    template struct value<double, lerp::cubic>;
    template struct value<double, lerp::quintic>;
} } }
