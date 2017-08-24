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

#ifndef __UTIL_GEOM_RAY_HPP
#define __UTIL_GEOM_RAY_HPP

#include "aabb.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "../vector.hpp"
#include "../point.hpp"


///////////////////////////////////////////////////////////////////////////////
namespace util::geom {
    template <size_t S, typename T>
    struct ray {
        constexpr ray () = default;

        constexpr ray (point<S,T> _origin, vector<S,T> _direction) noexcept:
            origin (_origin),
            direction (_direction)
        {
            CHECK (is_normalised (direction));
        }

        constexpr ray (point<S,T> _origin, point <S,T> _distant) noexcept:
            ray (_origin, _origin.to (_distant ))
        {
            CHECK (is_normalised (direction));
        }


        //---------------------------------------------------------------------
        // intersection tests
        T intersect (plane<S,T>) const;
        T intersect (AABB<S,T>) const;
        T intersect (sphere<S,T>) const;

        // queries
        T closest (point<S,T>) const;

        util::point<S,T> at (T) const;

        // data members
        point<S,T>  origin;
        vector<S,T> direction;
    };

    template <std::size_t S, typename T>
    constexpr auto
    make_ray (point<S,T> p, vector<S,T> d) noexcept
    {
        return ray<S,T> { p, d };
    }

    template <std::size_t S, typename T>
    constexpr auto
    make_ray (point<S,T> p, point<S,T> q)
    {
        return ray<S,T> { p, q };
    };

    typedef ray<2,float> ray2f;
    typedef ray<3,float> ray3f;
}

#endif
