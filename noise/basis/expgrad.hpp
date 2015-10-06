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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_NOISE_BASIS_EXPGRAD_HPP
#define __UTIL_NOISE_BASIS_EXPGRAD_HPP

#include "./gradient.hpp"

#include "../fwd.hpp"
#include "../../point.hpp"
#include "../../range.hpp"


///////////////////////////////////////////////////////////////////////////////
// modifies a standard uniforma gradient generator to follow an exponential
// distribution, base^(-t*exp)
//
// 
namespace util { namespace noise { namespace basis {
    template <
        size_t S,   // probe point dimensionality
        typename T // probe point value_type
    >
    struct expgrad : public gradient<S,T> {
        explicit expgrad (seed_t seed,
                          T base = (T)1.02,
                          T exponent = T{256});

        T base (void) const;
        T base (T);

        T exponent (void) const;
        T exponent (T);

    protected:
        vector<S,T> generate (pointi<S>) const;

        T m_base;
        T m_exponent;
    };
} } }

#include "expgrad.ipp"

#endif
