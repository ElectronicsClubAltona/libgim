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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_VECTOR_HPP
#define __UTIL_VECTOR_HPP

#include "json/tree.hpp"
#include "coord.hpp"

#include <array>
#include <iostream>
#include <initializer_list>

namespace util {
    template <size_t S, typename T>
    struct vector : public coord::base<S,T,vector,coord::xyzw,coord::stpq>
    {
        using coord::base<S,T,util::vector,coord::xyzw,coord::stpq>::base;

        // vector size
        bool is_zero (void) const;

        T magnitude  (void) const;
        T magnitude2 (void) const;

        T difference  (vector<S,T>) const;
        T difference2 (vector<S,T>) const;

        // normalisation
        bool is_normalised (void) const;

        vector<S,T>& normalise  (void);
        vector<S,T>  normalised [[gnu::warn_unused_result]] (void) const;

        // representations
        template <size_t D> vector<D,T> homog (void) const;

        // constants
        static const vector<S,T> UNIT;
        static const vector<S,T> ZERO;

        void sanity (void) const;
    };

    // polar/cartesian conversions; assumes (mag, angle) form.
    template <typename T> vector<2,T> polar_to_cartesian (vector<2,T>);
    template <typename T> vector<2,T> cartesian_to_polar (vector<2,T>);

    template <typename T> vector<3,T> cross (vector<3,T>, vector<3,T>);
    template <typename T> vector<3,T> spherical_to_cartesian (vector<3,T>);
    template <typename T> vector<3,T> cartesian_to_spherical (vector<3,T>);

    template <typename T> vector<2,T> to_euler   (vector<3,T>);
    template <typename T> vector<3,T> from_euler (vector<2,T>);

    // output and serialisation operators
    template <size_t S, typename T> std::ostream& operator<< (std::ostream&, vector<S,T>);

    template <size_t S, typename T>
    const json::tree::node& operator>> (const json::tree::node&, vector<S,T>&);

    // convenience typedefs
    template <typename T> using vector2 = vector<2,T>;
    template <typename T> using vector3 = vector<3,T>;
    template <typename T> using vector4 = vector<4,T>;

    typedef vector<2,size_t> vector2u;
    typedef vector<3,size_t> vector3u;

    typedef vector<2,intmax_t> vector2i;
    typedef vector<3,intmax_t> vector3i;

    typedef vector<2,float> vector2f;
    typedef vector<3,float> vector3f;
    typedef vector<4,float> vector4f;

    typedef vector<2,double> vector2d;
    typedef vector<3,double> vector3d;
    typedef vector<3,double> vector4d;
}

#include "vector.ipp"

#endif

