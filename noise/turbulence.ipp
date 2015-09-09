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
    template <typename T, typename D, typename P>
    turbulence<T,D,P>::turbulence (seed_t _seed,
                                   vector<2,T> _scale):
        data (_seed),
        perturb {
            hash::wang (_seed),
            hash::wang (hash::wang (_seed))
        },
        scale (_scale)
    { ; }

    ////////////////////////////////////////////////////////////////////////////
    template <typename T, typename D, typename P>
    typename turbulence<T,D,P>::seed_t
    turbulence<T,D,P>::seed (void) const
    {
        return data.seed ();
    }


    //-------------------------------------------------------------------------
    template <typename T, typename D, typename P>
    typename turbulence<T,D,P>::seed_t
    turbulence<T,D,P>::seed (seed_t _seed)
    {
        auto ret = _seed;

        data.seed (_seed);          _seed = hash::wang (_seed);
        perturb[0].seed (_seed);    _seed = hash::wang (_seed);
        perturb[1].seed (_seed);    _seed = hash::wang (_seed);

        return ret;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename D, typename P>
    constexpr T
    turbulence<T,D,P>::operator() (point<2,T> p) const
    {
        vector<2,T> n = {
            perturb[0] (p),
            perturb[1] (p)
        };

        // scale by the data frequency so that we match scale
        return data (p + n * scale / data.frequency ());
    }
} }
