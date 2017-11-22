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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_VECTOR_HPP
#define __UTIL_VECTOR_HPP

#include "coord.hpp"
#include "json/fwd.hpp"

namespace util {
    template <size_t S, typename T>
    struct vector : public coord::base<S,T,vector<S,T>>
    {
        using coord::base<S,T,vector<S,T>>::base;

        // representations
        template <size_t D> vector<D,T> homog (void) const;

        // constants
        static constexpr vector<S,T> ones  (void);
        static constexpr vector<S,T> zeros (void);

        void sanity (void) const;
    };

    template <typename T>
    constexpr
    vector<3,T>
    cross (vector<3,T>, vector<3,T>);

    template <typename T>
    constexpr
    T
    cross (vector<2,T>, vector<2,T>);

    // polar/cartesian conversions; assumes (mag, angle) form.
    template <typename T> vector<2,T> polar_to_cartesian (vector<2,T>);
    template <typename T> vector<2,T> cartesian_to_polar (vector<2,T>);

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

    template <size_t S> using vectoru = vector<S,unsigned>;
    template <size_t S> using vectori = vector<S,int>;
    template <size_t S> using vectorf = vector<S,float>;

    typedef vector2<unsigned> vector2u;
    typedef vector3<unsigned> vector3u;

    typedef vector2<int> vector2i;
    typedef vector3<int> vector3i;

    typedef vector2<float> vector2f;
    typedef vector3<float> vector3f;
    typedef vector4<float> vector4f;

    typedef vector2<double> vector2d;
    typedef vector3<double> vector3d;
    typedef vector3<double> vector4d;
}

#include "vector.ipp"

#endif

