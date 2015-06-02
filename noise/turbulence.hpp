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

#ifndef __UTIL_NOISE_TURBULENCE_HPP
#define __UTIL_NOISE_TURBULENCE_HPP

#include "../point.hpp"
#include "../vector.hpp"

#include <cstdint>

namespace util { namespace noise {
    /// perturb the evaluation point of a noise function by the results of
    /// a per-dimension noise function
    ///
    /// assumes the pertubation function is roughly symetrical around 0.
    /// nothing will explode if it isn't, but you'll see strong directional
    /// artefacts with higher scaling factors.
    template <typename T, typename D, typename P>
    struct turbulence {
        using seed_t = uint64_t;
        turbulence (seed_t, vector<2,T> scale);

        constexpr T operator() (point<2,T>) const;
        
        D data;
        P perturb[2];

        vector<2,T> scale;
    };
} }

#include "turbulence.ipp"

#endif
