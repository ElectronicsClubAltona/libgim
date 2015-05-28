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

#include "noise/basis.hpp"
#include "noise/lut.hpp"

#include "../debug.hpp"
#include "../point.hpp"
#include "../random.hpp"
#include "../vector.hpp"
#include "../hash/murmur/murmur2.hpp"

#include <algorithm>

using util::noise::basis;
using util::noise::value;
using util::noise::gradient;
using util::noise::cellular;


///////////////////////////////////////////////////////////////////////////////
// Generate a type from [-UNIT..UNIT]
template <typename T>
T
gen_scalar (util::point<2,T> p, uint64_t seed)
{
    using util::hash::murmur2::mix;

    T v = mix (seed, mix (uint64_t (p.y), uint64_t (p.x))) & 0xffff;
    v = v / T{0xffff} * 2 - 1;

    CHECK_GE (v, T{0});
    CHECK_LE (v, T{1});

    return v;
}


//-----------------------------------------------------------------------------
template <typename T>
util::vector<2,T>
gen_vector (util::point<2,T> p, uint64_t seed)
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
template <typename T>
basis<T>::basis (seed_t _seed):
    seed (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
basis<T>::basis ():
    seed (util::random<seed_t> ())
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
basis<T>::~basis ()
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
T
basis<T>::operator() (util::point<2,T>) const
{
    unreachable ();
}


//-----------------------------------------------------------------------------
namespace util { namespace noise {
    template struct basis<float>;
    template struct basis<double>;
} }


///////////////////////////////////////////////////////////////////////////////
template <typename T, util::noise::lerp_t<T> L>
value<T,L>::value (seed_t _seed):
    basis<T> (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
value<T,L>::value ()
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
    T p0 = gen_scalar<T> (p_int + util::vector<2,T>{ 0, 0 }, this->seed);
    T p1 = gen_scalar<T> (p_int + util::vector<2,T>{ 1, 0 }, this->seed);
    T p2 = gen_scalar<T> (p_int + util::vector<2,T>{ 0, 0 }, this->seed);
    T p3 = gen_scalar<T> (p_int + util::vector<2,T>{ 1, 1 }, this->seed);

    // Interpolate on one dimension, then the other.
    return L (L (p0, p1, p_rem.x),
              L (p2, p3, p_rem.x),
              p_rem.y);
}


//-----------------------------------------------------------------------------
namespace util { namespace noise {
        template struct value<float, lerp::trunc>;
        template struct value<float, lerp::linear>;
        template struct value<float, lerp::cubic>;
        template struct value<float, lerp::quintic>;

        template struct value<double, lerp::trunc>;
        template struct value<double, lerp::linear>;
        template struct value<double, lerp::cubic>;
        template struct value<double, lerp::quintic>;
} }


///////////////////////////////////////////////////////////////////////////////
template <typename T, util::noise::lerp_t<T> L>
gradient<T,L>::gradient (seed_t _seed):
    basis<T> (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
gradient<T,L>::gradient ()
{ ; }


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
util::range<T>
gradient<T,L>::bounds (void) const
    { return { -std::sqrt(T{2}) / 2, std::sqrt (T{2}) / 2 }; }


//-----------------------------------------------------------------------------
template <typename T, util::noise::lerp_t<T> L>
T
gradient<T,L>::operator() (util::point<2,T> p) const
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
    auto p0 = gen_vector<T> (p_int + util::vector<2,T> { 0, 0 }, this->seed).normalise ();
    auto p1 = gen_vector<T> (p_int + util::vector<2,T> { 1, 0 }, this->seed).normalise ();
    auto p2 = gen_vector<T> (p_int + util::vector<2,T> { 0, 1 }, this->seed).normalise ();
    auto p3 = gen_vector<T> (p_int + util::vector<2,T> { 1, 1 }, this->seed).normalise ();

    T v0 = p0.x *  p_rem.x      + p0.y *  p_rem.y;
    T v1 = p1.x * (p_rem.x - 1) + p1.y *  p_rem.y;
    T v2 = p2.x *  p_rem.x      + p2.y * (p_rem.y - 1);
    T v3 = p3.x * (p_rem.x - 1) + p3.y * (p_rem.y - 1);

    auto L0 = L (v0, v1, p_rem.x);
    auto L1 = L (v2, v3, p_rem.x);
    auto L_ = L (L0, L1, p_rem.y);
    return L_;
}


//-----------------------------------------------------------------------------
namespace util {
    namespace noise {
        template struct gradient<float, lerp::linear>;
        template struct gradient<float, lerp::cubic>;
        template struct gradient<float, lerp::quintic>;

        template struct gradient<double, lerp::linear>;
        template struct gradient<double, lerp::cubic>;
        template struct gradient<double, lerp::quintic>;
    }
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
cellular<T>::cellular (seed_t _seed):
    basis<T> (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
cellular<T>::cellular ()
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
util::range<T>
cellular<T>::bounds (void) const
    { return { 0.0, 1.5 }; }


//-----------------------------------------------------------------------------
template <typename T>
T
cellular<T>::operator() (util::point<2,T> p) const
{
    auto p_int = p.template cast<intmax_t> ();
    auto p_rem = p - p_int;

    // Generate the four corner values. It's not strictly necessary to
    // normalise the values, but we get a more consistent and visually
    // appealing range of outputs with normalised values.
    if (p.x < 0) { p_rem.x = 1 + p_rem.x; p_int.x -= 1; }
    if (p.y < 0) { p_rem.y = 1 + p_rem.y; p_int.y -= 1; }

    // +---+---+---+
    // | 0 | 1 | 2 |
    // +---+---+---+
    // | 3 | 4 | 5 |
    // +---+-------+
    // | 6 | 7 | 8 |
    // +---+---+---+

    T distances[9] = { std::numeric_limits<T>::quiet_NaN () };
    T *cursor = distances;

    for (signed y_off = -1; y_off <= 1 ; ++y_off)
        for (signed x_off = -1; x_off <= 1; ++x_off) {
            auto pos = vector<2,T> (T (x_off), T (y_off));
            auto off = gen_vector<T> (p_int + pos, this->seed);
            off += T{1};
            off /= T{2};

            CHECK (off.x >= 0 && off.x <= 1);
            CHECK (off.y >= 0 && off.y <= 1);

            pos += off;
            *cursor++ = pos.difference2 (p_rem);
        }

    std::sort (std::begin (distances), std::end (distances));
    CHECK_GE (distances[0], 0);
    CHECK (bounds ().contains (distances[0]));
    return distances[0];
}


//-----------------------------------------------------------------------------
namespace util { namespace noise {
    template struct cellular<float>;
    template struct cellular<double>;
} }
