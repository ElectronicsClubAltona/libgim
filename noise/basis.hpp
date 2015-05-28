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

#ifndef __UTIL_NOISE_BASIS_HPP
#define __UTIL_NOISE_BASIS_HPP

#include <cstdlib>
#include "lerp.hpp"
#include "../point.hpp"
#include "../range.hpp"

namespace util { namespace noise {
    template <typename T> using lerp_t = T (*)(T,T,T);


    template <typename T>
    struct basis {
        typedef uint64_t seed_t;

        basis (seed_t);
        basis ();
        virtual ~basis ();

        seed_t seed;

        virtual range<T> bounds (void) const = 0;
        virtual T operator() (util::point<2,T>) const = 0;
    };


    /// Single value per grid space
    template <typename T, lerp_t<T>>
    struct value : public basis<T> {
        using seed_t = typename basis<T>::seed_t;

        value (seed_t);
        value ();

        virtual range<T> bounds (void) const final;
        virtual T operator() (util::point<2,T>) const final;
    };


    /// Perlin: interpolated value across each grid space
    template <typename T, lerp_t<T> L>
    struct gradient : public basis<T> {
        using seed_t = typename basis<T>::seed_t;

        gradient (seed_t);
        gradient ();

        virtual range<T> bounds (void) const final;
        virtual T operator() (util::point<2,T>) const final;
    };


    /// Cellular/Worley/Vornoi of order-1
    template <typename T>
    struct cellular : public basis<T> {
        using seed_t = typename basis<T>::seed_t;

        cellular (seed_t);
        cellular ();

        virtual range<T> bounds (void) const final;
        virtual T operator() (util::point<2,T>) const final;
    };
} }

#endif
