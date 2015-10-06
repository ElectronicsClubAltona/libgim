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

#ifdef __UTIL_NOISE_TURBULENCE_IPP
#error
#endif
#define __UTIL_NOISE_TURBULENCE_IPP

#include "../hash/wang.hpp"


namespace util { namespace noise {
    ///////////////////////////////////////////////////////////////////////////
    template <typename D, typename P>
    turbulence<D,P>::turbulence (seed_t  _seed,
                                     scale_t _scale):
        data  (_seed),
        scale (_scale)
    {
        for (auto &p: perturb)
            new (&p) P (_seed = hash::wang (_seed));
    }

    ////////////////////////////////////////////////////////////////////////////
    template <typename D, typename P>
    seed_t
    turbulence<D,P>::seed (void) const
    {
        return data.seed ();
    }


    //-------------------------------------------------------------------------
    template <typename D, typename P>
    seed_t
    turbulence<D,P>::seed (seed_t _seed)
    {
        auto ret = _seed;
        data.seed (_seed);

        for (size_t i = 0; i < S; ++i)
            perturb[i].seed (_seed = hash::wang (_seed));

        return ret;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename D, typename P>
    constexpr typename turbulence<D,P>::value_t
    turbulence<D,P>::operator() (point_t p) const
    {
        scale_t n;
        for (size_t i = 0; i < S; ++i)
            n[i] = perturb[i] (p);

        // scale by the data frequency so that we match scale
        return data (p + n * scale / data.frequency ());
    }
} }
