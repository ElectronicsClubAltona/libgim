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
    template <size_t S, typename T>
    patch<S,T>::patch (seed_t _seed, T _width):
        m_width (_width),
        m_power (exactly_zero (_width)
                 ? std::numeric_limits<T>::infinity ()
                 : std::log (THRESHOLD) / std::log (1 - _width)),
        m_seed (_seed)
    { ; }


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    range<T>
    patch<S,T>::bounds (void) const
    {
        return { T{0}, T{1} };
    }


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    T
    patch<S,T>::operator () (point<S,T> p) const
    {
        static const size_t COUNT = type::distance<S,2>::OFFSET_SIZE;

        // extract integer and fractional parts. be careful to always round down
        auto p_int = floor (p).template cast<intmax_t> ();
        auto p_rem = (p - p_int).template as<point> ();

        // find the distances to each neighbour's centroid.
        util::point<S,T> centres[COUNT];
        std::transform (std::begin (this->OFFSETS),
                        std::end   (this->OFFSETS),
                        std::begin (centres),
                        [this,p_int] (auto i) { return rand::coord<point,T> (m_seed, p_int + i) + i; });

        T distances[COUNT];
        std::transform (std::begin (centres),
                        std::end   (centres),
                        std::begin (distances),
                        [p_rem] (auto i) { return util::distance (p_rem, i); });

        // sort the distances using indices so we can reuse indices into
        // 'OFFSETS' to generate the random patch values
        unsigned indices[COUNT];
        std::iota (std::begin (indices), std::end (indices), 0);

        std::sort (std::begin (indices),
                   std::end   (indices),
                   [&] (auto a, auto b) {
            return distances[a] < distances[b];
        });

        // setup normalisation for the distances to the nearest points. the
        // neighbourhood size is implicitly specified by the 1.5 unit maximum
        // distance.
        const size_t hi_off = pow(3,S);

        const auto lo = distances[indices[0     ]];
        const auto hi = distances[indices[hi_off]];

        T out  = 0.f;
        T sumw = 0.f;

        // sum the weight values of each neighbour. weight by a function of
        // the distance. we use an power function which allows a known width
        // to blend.
        for (size_t i = 0; i < hi_off; ++i)
        {
            auto v = rand::scalar<T> (
                m_seed,
                p_int + this->OFFSETS[indices[i]]
            );

            auto d = (distances[indices[i]] - lo) / (hi - lo);
            auto w = std::pow (1 - d, m_power);

            sumw += w;
            out += v * w;
        }

        return out / sumw;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    seed_t
    patch<S,T>::seed (void) const
    {
        return m_seed;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    seed_t
    patch<S,T>::seed (util::noise::seed_t _seed)
    {
        return m_seed = _seed;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    T
    patch<S,T>::width (void) const
    {
        return m_width;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    T
    patch<S,T>::width (T _width)
    {
        m_width = _width;
        m_power = exactly_zero (_width)
                  ? std::numeric_limits<T>::infinity ()
                  : std::log (THRESHOLD) / std::log (1 - _width);

        return m_width;
    }
} } }
