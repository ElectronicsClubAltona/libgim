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

#ifdef __UTIL_NOISE_BASIS_WORLEY_IPP
#error
#endif
#define __UTIL_NOISE_BASIS_WORLEY_IPP


#include "../../debug.hpp"


namespace util { namespace noise { namespace basis {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, size_t F>
    worley<T,F>::worley (seed_t _seed):
        m_seed (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, size_t F>
    util::range<T>
    worley<T,F>::bounds (void) const
    {
        return { 0.0, 1.5 };
    }


    //-------------------------------------------------------------------------
    template <typename T, size_t F>
    seed_t
    worley<T,F>::seed (void) const
    {
        return m_seed;
    }


    //-------------------------------------------------------------------------
    template <typename T, size_t F>
    seed_t
    worley<T,F>::seed (seed_t _seed)
    {
        return m_seed = _seed;
    }


    //-------------------------------------------------------------------------
    template <typename T, size_t F>
    T
    worley<T,F>::operator() (util::point<2,T> p) const
    {
        // extract integer and fractional parts. be careful to always round down
        auto p_int = floor (p).template cast<intmax_t> ();
        auto p_rem = (p - p_int).template as<point> ();

        // setup an array of distances
        constexpr size_t RADIUS = 1;
        constexpr size_t COUNT = pow2 (RADIUS * 2 + 1);
        T distances[COUNT] = { std::numeric_limits<T>::quiet_NaN () };
        T *cursor = distances;

        // record the distances to each candidate point
        for (signed y_off = -signed(RADIUS); y_off <= signed(RADIUS) ; ++y_off) {
            for (signed x_off = -signed(RADIUS); x_off <= signed(RADIUS); ++x_off) {
                auto off = vector<2,intmax_t> {x_off, y_off};
                auto pos = generate (p_int + off);

                CHECK_LIMIT (pos.x, T{0}, T{1});
                CHECK_LIMIT (pos.y, T{0}, T{1});

                *cursor = distance2 (pos + off, p_rem);
                cursor++;
            }
        }

        // find the f'th lowest value
        static_assert (F < COUNT, "worley order must be less than search radius");
        std::partial_sort (distances, distances + F, distances + COUNT);
        CHECK_GE (distances[F], 0);
        return distances[F];
    }


    //////////////////////////////////////////////////////////////////////////
    template <typename T, size_t F>
    point<2,T>
    worley<T,F>::generate (point<2,intmax_t> p) const
    {
        using util::hash::murmur2::mix;

        auto u = mix (m_seed, mix (uint64_t (p.x), uint64_t (p.y)));
        auto v = mix (u, m_seed);

        auto r = util::point<2,T> {
            (u & 0xffff) / T{0xffff},
            (v & 0xffff) / T{0xffff}
        };

        CHECK_LIMIT (r, T{0}, T{1});
        return r;
    }
} } }
