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
 * Copyright 2011-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_VECTOR_HPP
#define CRUFT_UTIL_VECTOR_HPP

#include "./coord/fwd.hpp"
#include "./coord.hpp"

#include "json/fwd.hpp"

#include <cstddef>
#include <cmath>


///////////////////////////////////////////////////////////////////////////////
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

    // convert vector in spherical coordinates (r,theta,phi) with theta
    // inclination and phi azimuth to cartesian coordinates (x,y,z)
    template <typename T>
    constexpr vector<3,T>
    spherical_to_cartesian (const vector<3,T> s)
    {
        return {
            s.x * std::sin (s.y) * std::cos (s.z),
            s.x * std::sin (s.y) * std::sin (s.z),
            s.x * std::cos (s.y)
        };
    }

    // convert vector in cartesian coordinates (x,y,z) to spherical
    // coordinates (r,theta,phi) with theta inclination and phi azimuth.
    template <typename T>
    constexpr vector<3,T>
    cartesian_to_spherical (vector<3,T> c)
    {
        auto r = norm (c);
        return {
            r,
            std::acos (c.z / r),
            std::atan (c.y / c.z)
        };
    }

    template <typename T> vector<2,T> to_euler   (vector<3,T>);
    template <typename T> vector<3,T> from_euler (vector<2,T>);

    // output and serialisation operators
    template <size_t S, typename T>
    const json::tree::node&
    operator>> (const json::tree::node&, vector<S,T>&);

    template <typename T> using vector1 = vector<1,T>;
    template <typename T> using vector2 = vector<2,T>;
    template <typename T> using vector3 = vector<3,T>;
    template <typename T> using vector4 = vector<4,T>;

    template <size_t S> using vectoru = vector<S,unsigned>;
    template <size_t S> using vectori = vector<S,int>;
    template <size_t S> using vectorf = vector<S,float>;
    template <std::size_t S> using vectorb = vector<S,bool>;

    using vector2u = vector2<unsigned>;
    using vector3u = vector3<unsigned>;
    using vector4u = vector4<unsigned>;

    using vector2i = vector2<int>;
    using vector3i = vector3<int>;
    using vector4i = vector4<int>;

    using vector1f = vector1<float>;
    using vector2f = vector2<float>;
    using vector3f = vector3<float>;
    using vector4f = vector4<float>;

    using vector2d = vector2<double>;
    using vector3d = vector3<double>;
    using vector4d = vector4<double>;

    using vector2b = vector2<bool>;
    using vector3b = vector3<bool>;
    using vector4b = vector4<bool>;
}

#include "vector.ipp"

#endif

