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


#ifndef __UTIL_NOISE_BASIS_PERLIN_HPP
#define __UTIL_NOISE_BASIS_PERLIN_HPP

#include "./gradient.hpp"
#include "./type/gradient.hpp"

#include "../fwd.hpp"
#include "../../point.hpp"
#include "../../range.hpp"

namespace util { namespace noise { namespace basis {
    /// Perlin: interpolated value across each grid space
    template <
        size_t S,       // point point dimensionality
        typename T,     // arithmetic and result value_type, must be floating point
        lerp_t<T> L,    // gradient interpolation function
        template <      // gradient provider class, must provide generate(point_t)
            size_t,
            typename,
            lerp_t<T>
        > class G = gradient
    >
    struct perlin : public G<S,T,L>, public type::gradient<S> {
        perlin (seed_t);

        range<T> bounds (void) const;

        T operator() (point<S,T>) const;
    };
} } }

#include "perlin.ipp"

#endif
