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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#if defined(__UTIL_NOISE_BASIS_PATCH_IPP)
#error
#endif
#define __UTIL_NOISE_BASIS_PATCH_IPP

#include "../../types.hpp"
#include "../../ray.hpp"
#include "../../vector.hpp"


namespace util { namespace noise { namespace basis {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    patch<T>::patch (seed_t _seed, T _width):
        m_width (_width),
        m_power (exactly_zero (_width)
                 ? std::numeric_limits<T>::infinity ()
                 : std::log (THRESHOLD) / std::log (1 - _width)),
        m_seed (_seed)
    { ; }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    range<T>
    patch<T>::bounds (void) const
    {
        return { T{0}, T{1} };
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    T
    patch<T>::operator () (point2<T> p) const
    {
        // extract integer and fractional parts. be careful to always round down
        // (particularly with negatives) and avoid rounding errors.
        auto p_int = p.template cast<intmax_t> ();
        if (p.x < 0) p_int.x -= 1;
        if (p.y < 0) p_int.y -= 1;
        auto p_rem = (p - p_int).template as<point> ();

        static const util::vector2i OFFSETS[] = {
                                    {  0, -2 },
                        { -1, -1 }, {  0, -1 }, {  1, -1 },
            { -2,  0 }, { -1,  0 }, {  0,  0 }, {  1,  0 }, {  2,  0 },
                        { -1,  1 }, {  0,  1 }, {  1,  1 },
                                    {  0,  2 },
        };

        static const size_t COUNT = elems (OFFSETS);

        // find the distances to each neighbour's centroid
        util::point2<T> centres[COUNT];
        for (size_t i = 0; i < COUNT; ++i)
            centres[i] = centroid (p_int + OFFSETS[i]) + OFFSETS[i];

        T distances[COUNT];
        for (size_t i = 0; i < COUNT; ++i)
            distances[i] = std::sqrt (util::distance2 (p_rem, centres[i]));

        // sort the distances, using indices so we can use 'offsets' to generate values
        unsigned indices[COUNT];
        std::iota (std::begin (indices), std::end (indices), 0);

        std::sort (std::begin (indices),
                   std::end (indices),
                   [&] (auto a, auto b) {
            return distances[a] < distances[b];
        });

        // calculate normalisation constants for the 9 nearest points. the
        // neighbourhood size is implicitly specified by the 1.5 unit maximum
        // distance.
        constexpr auto MAX_DISTANCE = 2.1213203435596424f; // std::hypot (1.5f, 1.5f);
        const auto lo = distances[indices[0]];
        const auto hi = std::min (distances[indices[COUNT-1]], MAX_DISTANCE);

        T out  = 0.f;
        T sumw = 0.f;

        // sum the weight values of each neighbour. weight by a function of
        // the distance. we use an power function which allows a known width
        // to blend.
        for (size_t i = 0; i < COUNT && distances[indices[i]] <= MAX_DISTANCE; ++i)
        {
            auto v = generate (p_int + OFFSETS[indices[i]]);
            auto d = (distances[indices[i]] - lo) / (hi - lo);
            auto w = std::pow (1-d, m_power);

            sumw += w;
            out += v * w;
        }

        return out / sumw;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    seed_t
    patch<T>::seed (void) const
    {
        return m_seed;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    seed_t
    patch<T>::seed (util::noise::seed_t _seed)
    {
        return m_seed = _seed;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    T
    patch<T>::width (void) const
    {
        return m_width;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    T
    patch<T>::width (T _width)
    {
        m_width = _width;
        m_power = exactly_zero (_width)
                  ? std::numeric_limits<T>::infinity ()
                  : std::log (THRESHOLD) / std::log (1 - _width);

        return m_width;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    util::point2<T>
    patch<T>::centroid (util::point2i p) const
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


    //-------------------------------------------------------------------------
    template <typename T>
    T
    patch<T>::generate (util::point2i p) const
    {
        using util::hash::murmur2::mix;

        auto u = mix (m_seed, mix (uint64_t (p.x), uint64_t (p.y)));
        return (u & 0xffff) / T{0xffff};
    }
} } }
