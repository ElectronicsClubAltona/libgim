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

#ifdef __UTIL_NOISE_BASIS_VALUE_IPP
#error
#endif
#define __UTIL_NOISE_BASIS_VALIE_IPP

#include "../rand.hpp"
#include "../../types.hpp"


namespace util { namespace noise { namespace basis {
    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T, template <typename> class L>
    value<S,T,L>::value (seed_t _seed):
        m_seed (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, template <typename> class L>
    util::range<T>
    value<S,T,L>::bounds (void) const
    {
        return { -1, 1 };
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, template <typename> class L>
    seed_t
    value<S,T,L>::seed (void) const
    {
        return m_seed;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, template <typename> class L>
    seed_t
    value<S,T,L>::seed (seed_t _seed)
    {
        return m_seed = _seed;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, template <typename> class L>
    T
    value<S,T,L>::operator() (util::point<S,T> p) const
    {
        // extract integer and fractional parts. be careful to always round down
        auto p_int = floor (p).template cast<intmax_t> ();
        auto p_rem = p - p_int;

        // generate the corner points
        std::array<pointi<S>,pow(2,S)> p_;
        std::transform (std::begin (this->CORNERS), std::end (this->CORNERS),
                        std::begin (p_),
                        [p_int] (auto i) { return p_int + i; });

        // Generate the corner values
        std::array<T,pow(2,S)> g_;
        std::transform (std::begin (p_), std::end (p_),
                        std::begin (g_),
                        [this] (auto i) { return rand::scalar<value_t> (m_seed, i) * 2 - 1; });

        // Interpolate on one dimension, then the other.
        T l_[pow(2,S)];
        std::copy (std::begin (g_), std::end (g_), std::begin (l_));

        for (size_t i = S; i; --i)
            for (size_t j = 0; j < std::pow(2,i); j += 2)
                l_[j / 2] = L<T>() (l_[j], l_[j+1], p_rem[S-i]);
        return l_[0];
    }
} } }
