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
generate (intmax_t x, intmax_t y, uint64_t seed)
{
    using util::hash::murmur2::mix;

    T v = mix (seed, mix (uint64_t (y), uint64_t (x))) & 0xffff;
    v = v / T{0xffff} * 2 - 1;

    CHECK_GE (v, T{0});
    CHECK_LE (v, T{1});

    return v;
}


//-----------------------------------------------------------------------------
template <>
util::vector2d
generate (intmax_t x, intmax_t y, uint64_t seed)
{
    using util::hash::murmur2::mix;

    auto u = mix (seed, mix (uint64_t (x), uint64_t (y)));
    auto v = mix (u, seed);

    auto r = util::vector2d {
        (u & 0xffff) / double{0xffff},
        (v & 0xffff) / double{0xffff}
    } * 2.0 - 1.0;

    CHECK_GE (r, double{-1});
    CHECK_LE (r, double{ 1});

    return r;
}

//-----------------------------------------------------------------------------
template <>
util::vector2f
generate (intmax_t x, intmax_t y, uint64_t seed)
{
    using util::hash::murmur2::mix;

    auto u = mix (seed, mix (uint64_t (x), uint64_t (y)));
    auto v = mix (u, seed);

    auto r = util::vector2f {
        (u & 0xffff) / float{0xffff},
        (v & 0xffff) / float{0xffff}
    } * 2.f - 1.f;

    CHECK_GE (r, float (-1));
    CHECK_LE (r, float ( 1));

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
basis<T>::operator() (T, T) const
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
value<T,L>::operator() (T x, T y) const
{
    intmax_t x_int = static_cast<intmax_t> (x);
    intmax_t y_int = static_cast<intmax_t> (y);
    T x_fac = x - x_int;
    T y_fac = y - y_int;

    // Shift the coordinate system down a little to ensure we get unit weights
    // for the lerp. It's better to do this than abs the fractional portion so
    // we don't get reflections along the origin.
    if (x < 0) { x_fac = 1 + x_fac; x_int -= 1; }
    if (y < 0) { y_fac = 1 + y_fac; y_int -= 1; }

    // Generate the four corner values
    T p0 = generate<T> (x_int,     y_int,     this->seed);
    T p1 = generate<T> (x_int + 1, y_int,     this->seed);
    T p2 = generate<T> (x_int,     y_int + 1, this->seed);
    T p3 = generate<T> (x_int + 1, y_int + 1, this->seed);

    // Interpolate on one dimension, then the other.
    return L (L (p0, p1, x_fac),
              L (p2, p3, x_fac),
              y_fac);
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
gradient<T,L>::operator() (T x, T y) const
{
    intmax_t x_int = static_cast<intmax_t> (x);
    intmax_t y_int = static_cast<intmax_t> (y);
    T x_fac = x - x_int;
    T y_fac = y - y_int;

    // Shift the coordinate system down a little to ensure we get unit weights
    // for the lerp. It's better to do this than abs the fractional portion so
    // we don't get reflections along the origin.
    if (x < 0) { x_fac = 1 + x_fac; x_int -= 1; }
    if (y < 0) { y_fac = 1 + y_fac; y_int -= 1; }

    // Generate the four corner values. It's not strictly necessary to
    // normalise the values, but we get a more consistent and visually
    // appealing range of outputs with normalised values.
    auto p0 = generate<vector<2,T>> (x_int,     y_int,       this->seed).normalise ();
    auto p1 = generate<vector<2,T>> (x_int + 1, y_int,       this->seed).normalise ();
    auto p2 = generate<vector<2,T>> (x_int,     y_int + 1,   this->seed).normalise ();
    auto p3 = generate<vector<2,T>> (x_int + 1, y_int + 1,   this->seed).normalise ();

    T v0 = p0.x *      x_fac  + p0.y *  y_fac;
    T v1 = p1.x * (x_fac - 1) + p1.y *  y_fac;
    T v2 = p2.x *      x_fac  + p2.y * (y_fac - 1);
    T v3 = p3.x * (x_fac - 1) + p3.y * (y_fac - 1);

    return L (L (v0, v1, x_fac),
              L (v2, v3, x_fac),
              y_fac);
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
cellular<T>::operator() (T x, T y) const
{
    intmax_t x_int = static_cast<intmax_t> (x);
    intmax_t y_int = static_cast<intmax_t> (y);
    T x_fac = x - x_int;
    T y_fac = y - y_int;

    // Generate the four corner values. It's not strictly necessary to
    // normalise the values, but we get a more consistent and visually
    // appealing range of outputs with normalised values.
    if (x < 0) { x_fac = 1 + x_fac; x_int -= 1; }
    if (y < 0) { y_fac = 1 + y_fac; y_int -= 1; }

    // +---+---+---+
    // | 0 | 1 | 2 |
    // +---+---+---+
    // | 3 | 4 | 5 |
    // +---+-------+
    // | 6 | 7 | 8 |
    // +---+---+---+

    point<2,T> centre = { x_fac, y_fac };
    T distances[9] = { std::numeric_limits<T>::quiet_NaN () };
    T *cursor = distances;

    for (signed y_off = -1; y_off <= 1 ; ++y_off)
        for (signed x_off = -1; x_off <= 1; ++x_off) {
            auto pos = point<2,T> (T (x_off), T (y_off));
            auto off = generate<vector<2,T>> (x_int + x_off, y_int + y_off, this->seed);
            off += T{1};
            off /= T{2};

            CHECK (off.x >= 0 && off.x <= 1);
            CHECK (off.y >= 0 && off.y <= 1);

            pos += off;
            *cursor++ = pos.distance2 (centre);
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
