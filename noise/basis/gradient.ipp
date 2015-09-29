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

#ifdef __UTIL_NOISE_BASIS_GRADIENT_IPP
#error
#endif
#define __UTIL_NOISE_BASIS_GRADIENT_IPP

#include "../rand.hpp"

namespace util { namespace noise { namespace basis {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, util::noise::lerp_t<T> L>
    gradient<T,L>::gradient (seed_t _seed):
        m_seed (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, lerp_t<T> L>
    seed_t
    gradient<T,L>::seed (void) const
    {
        return m_seed;
    }


    //-------------------------------------------------------------------------
    template <typename T, lerp_t<T> L>
    seed_t
    gradient<T,L>::seed (seed_t _seed)
    {
        return m_seed = _seed;
    }


    //-------------------------------------------------------------------------
    template <typename T, util::noise::lerp_t<T> L>
    vector<2,T>
    gradient<T,L>::generate (point<2,intmax_t> p) const
    {
        return noise::rand<2,T> (m_seed, p);
    }
} } }

