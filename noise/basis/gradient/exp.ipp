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

#ifdef __UTIL_NOISE_BASIS_EXP_IPP
#error
#endif
#define __UTIL_NOISE_BASIS_EXP_IPP

#include "../../rand.hpp"

#include <cmath>

namespace util { namespace noise { namespace basis { namespace gradient {
    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    exp<S,T>::exp (seed_t _seed, T _base, T _exponent):
        uniform<S,T> (_seed),
        m_base (_base),
        m_exponent (_exponent)
    { ; }


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    vector<S,T>
    exp<S,T>::generate (pointi<S> p) const
    {
        auto t = rand::scalar<float> (this->seed (), p);
        auto factor = std::pow (m_base, -t * m_exponent);
        return factor * uniform<S,T>::generate (p);
    }
} } } }
