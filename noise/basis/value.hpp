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


#ifndef __UTIL_NOISE_BASIS_VALUE_HPP
#define __UTIL_NOISE_BASIS_VALUE_HPP

#include "./type/gradient.hpp"

#include "../fwd.hpp"
#include "../../range.hpp"
#include "../../point.hpp"

namespace util { namespace noise { namespace basis {
    /// Single value per grid space
    template <
        size_t S,
        typename T,
        template <
            typename
        > class L
    >
    struct value : public type::gradient<S> {
        using value_t = T;
        using point_t = point<S,T>;

        value (seed_t);

        range<T> bounds (void) const;

        value_t operator() (point_t) const;

        seed_t seed (void) const;
        seed_t seed (seed_t);

    private:
        seed_t m_seed;
    };
} } }

#include "value.ipp"

#endif
