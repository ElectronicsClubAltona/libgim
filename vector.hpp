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
#include <initializer_list>

namespace util {
    template <size_t S, typename T>
    struct vector : public coord::base<S,T,vector,coord::xyzw,coord::stpq>
    {
        using coord::base<S,T,util::vector,coord::xyzw,coord::stpq>::base;

        // representations
        template <size_t D> vector<D,T> homog (void) const;

        // constants
        static const vector<S,T> ONES;
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
    template <size_t S, typename T>
    const json::tree::node&
    operator>> (const json::tree::node&, vector<S,T>&);

    // convenience typedefs
    template <typename T> using vector2 = vector<2,T>;
    template <typename T> using vector3 = vector<3,T>;
    template <typename T> using vector4 = vector<4,T>;

    template <size_t S> using vectoru = vector<S,uintmax_t>;
    template <size_t S> using vectori = vector<S,intmax_t>;
    template <size_t S> using vectorf = vector<S,float>;

    typedef vector2<size_t> vector2u;
    typedef vector3<size_t> vector3u;

    typedef vector2<intmax_t> vector2i;
    typedef vector3<intmax_t> vector3i;

    typedef vector2<float> vector2f;
    typedef vector3<float> vector3f;
    typedef vector4<float> vector4f;

    typedef vector2<double> vector2d;
    typedef vector3<double> vector3d;
    typedef vector3<double> vector4d;
}

#include "vector.ipp"

#endif

