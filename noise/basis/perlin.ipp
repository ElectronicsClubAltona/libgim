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

#ifdef __UTIL_NOISE_BASIS_PERLIN_IPP
#error
#endif
#define __UTIL_NOISE_BASIS_PERLIN_IPP

#include "../../types.hpp"


namespace util { namespace noise { namespace basis {
    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T, util::noise::lerp_t<T> L, template <size_t,typename,lerp_t<T>> class G>
    perlin<S,T,L,G>::perlin (seed_t _seed):
        G<S,T,L> (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, util::noise::lerp_t<T> L, template <size_t,typename,lerp_t<T>> class G>
    util::range<T>
    perlin<S,T,L,G>::bounds (void) const
    {
        return {
            -std::sqrt (T{2}) / 2,
             std::sqrt (T{2}) / 2
        };
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, util::noise::lerp_t<T> L, template <size_t,typename,lerp_t<T>> class G>
    T
    perlin<S,T,L,G>::operator() (util::point<S,T> p) const
    {
        // extract integer and fractional parts. be careful to always round down
        auto p_int = floor (p).template cast<intmax_t> ();
        auto p_rem = p - p_int;

        // generate the corner positions
        pointi<S> p_[pow(2,S)];
        std::transform (std::begin (this->CORNERS), std::end (this->CORNERS),
                        std::begin (p_),
                        [p_int] (auto i) { return i + p_int; });

        // generate the corner gradients
        vector<S,T> g_[pow(2,S)];
        std::transform (std::begin (p_), std::end (p_),
                        std::begin (g_),
                        [this] (auto i) { return this->generate (i); });

        // compute the dot products
        T v_[pow(2,S)];
        for (size_t i = 0; i < elems (v_); ++i)
            v_[i] = dot (g_[i], p - p_[i]);

        // interpolate the results
        T l_[pow(2,S)];
        std::copy (std::begin (v_), std::end (v_), std::begin (l_));

        for (size_t i = S; i; --i)
            for (size_t j = 0; j < std::pow(2,i); j += 2)
                l_[j / 2] = L (l_[j], l_[j+1], p_rem[S-i]);
        return l_[0];
    }
} } }
