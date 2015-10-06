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
    template <size_t S, typename T, size_t F>
    worley<S,T,F>::worley (seed_t _seed):
        m_seed (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, size_t F>
    util::range<T>
    worley<S,T,F>::bounds (void) const
    {
        return { 0.0, 1.5 };
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, size_t F>
    seed_t
    worley<S,T,F>::seed (void) const
    {
        return m_seed;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, size_t F>
    seed_t
    worley<S,T,F>::seed (seed_t _seed)
    {
        return m_seed = _seed;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, size_t F>
    T
    worley<S,T,F>::operator() (point<S,T> p) const
    {
        // extract integer and fractional parts. be careful to always round down
        auto p_int = floor (p).template cast<intmax_t> ();
        auto p_rem = (p - p_int).template as<point> ();

        // setup an array of distances
        constexpr size_t COUNT = type::distance<S,1>::OFFSET_SIZE;
        T distances[COUNT];

        std::transform (std::begin (this->OFFSETS), std::end (this->OFFSETS),
                        distances,
                        [p_int,p_rem,this] (auto i) {
            auto q = this->generate (p_int + i);
            return distance2 (q + i, p_rem);
        });

        // find the f'th lowest value
        static_assert (F < COUNT, "worley order must be less than search radius");
        std::partial_sort (distances, distances + F, distances + COUNT);
        CHECK_GE (distances[F], 0);
        return distances[F];
    }


    //////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T, size_t F>
    point<S,T>
    worley<S,T,F>::generate (point<S,intmax_t> p) const
    {
        return (noise::rand<util::point,T> (m_seed, p) + 1) / 2;
    }
} } }
