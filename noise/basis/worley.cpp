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

#include "worley.hpp"

#include "../../hash/murmur/murmur2.hpp"

using util::noise::basis::worley;


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
template <typename T>
worley<T>::worley (seed_t _seed):
    seed (_seed)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
worley<T>::worley ():
    seed (time (nullptr))
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
util::range<T>
worley<T>::bounds (void) const
{
    return { 0.0, 1.5 };
}


//-----------------------------------------------------------------------------
template <typename T>
T
worley<T>::operator() (util::point<2,T> p) const
{
    // extract integer and fractional parts. be careful to always round down
    // (particularly with negatives) and avoid rounding errors.
    auto p_int = p.template cast<intmax_t> ();
    if (p.x < 0) p_int.x -= 1;
    if (p.y < 0) p_int.y -= 1;
    auto p_rem = abs (p - p_int);

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
            auto off = generate<T> (p_int + pos, this->seed);
            off += T{1};
            off /= T{2};

            CHECK (off.x >= 0 && off.x <= 1);
            CHECK (off.y >= 0 && off.y <= 1);

            pos += off;
            *cursor = distance2 (pos + off, p_rem);
            cursor++;
        }
    }

    std::sort (std::begin (distances), std::end (distances));
    CHECK_GE (distances[0], 0);
    CHECK (bounds ().contains (distances[0]));
    return distances[0];
}


///////////////////////////////////////////////////////////////////////////////
#include "../lerp.hpp"

namespace util { namespace noise { namespace basis {
    template struct worley<float>;
    template struct worley<double>;
} } }
