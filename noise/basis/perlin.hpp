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

#include "./gradient/uniform.hpp"
#include "./type/gradient.hpp"

#include "../fwd.hpp"
#include "../../point.hpp"
#include "../../range.hpp"

namespace util { namespace noise { namespace basis {
    /// Perlin: interpolated value across each grid space
    template <
        size_t S,       // point point dimensionality
        typename T,     // arithmetic and result value_type, must be floating point
        template <      // gradient interpolation function
            typename
        > class L,
        template <      // gradient provider class, must provide generate(point_t) 
            size_t,
            typename   
        > class G = gradient::uniform
    >
    struct perlin : public G<S,T>, public type::gradient<S> {
        using value_t = T;
        using point_t = point<S,T>;

        perlin (seed_t);

        range<T> bounds (void) const;

        T operator() (point_t) const noexcept;
    };
} } }

#include "perlin.ipp"

#endif
