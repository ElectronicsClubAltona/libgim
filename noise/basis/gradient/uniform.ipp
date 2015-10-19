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

#ifdef __UTIL_NOISE_BASIS_GRADIENT_UNIFORM_IPP
#error
#endif
#define __UTIL_NOISE_BASIS_GRADIENT_UNIFORM_IPP

#include "../../rand.hpp"

namespace util { namespace noise { namespace basis { namespace gradient {
    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    uniform<S,T>::uniform (seed_t _seed):
        m_seed (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    seed_t
    uniform<S,T>::seed (void) const
    {
        return m_seed;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    seed_t
    uniform<S,T>::seed (seed_t _seed)
    {
        return m_seed = _seed;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    vector<S,T>
    uniform<S,T>::generate (pointi<S> p) const
    {
        return rand::coord<vector,T> (m_seed, p) * 2 - 1;
    }
} } } }