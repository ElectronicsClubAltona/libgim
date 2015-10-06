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
    template <
        typename D, // data fractal
        typename P  // pertubation fractal
    >
    struct turbulence {
        using value_t = typename D::value_t;
        using point_t = typename D::point_t;
        using scale_t = vector<point_t::dimension,value_t>;

        static constexpr size_t S = D::point_t::dimension;

        static_assert (std::is_same<typename D::value_t, typename P::value_t>::value,
                       "data and perturbation value types must match");
        static_assert (std::is_same<typename D::point_t, typename P::point_t>::value,
                       "data and perturbation point types must match");

        turbulence (seed_t, scale_t);

        seed_t seed (seed_t);
        seed_t seed (void) const;

        constexpr value_t operator() (point_t) const;

        D data;

        // XXX: use a union to defer initialization of pertubation fractals in
        // the constructor. i know this is horrible, but there's no time to
        // write the proper generator constructor to pass out the seeds.
        union {
            char _[0];
            P perturb[S];
        };

        scale_t scale;
    };
} }

#include "turbulence.ipp"

#endif
