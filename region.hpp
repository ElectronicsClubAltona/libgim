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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_REGION_HPP
#define __UTIL_REGION_HPP

#include "extent.hpp"
#include "point.hpp"
#include "vector.hpp"
#include "types/traits.hpp"

namespace util {
    /**
     * A two-dimensional rectangle, with size and position.
     */
    template <size_t S, typename T>
    struct region {
        using position_type = T;
        using size_type = typename try_unsigned<T>::type;

        using extent_t = util::extent<S,size_type>;
        using point_t  = util::point<S,position_type>;

        using value_type = T;

        //---------------------------------------------------------------------
        static constexpr size_t dimension = S;
        static constexpr size_t elements = extent_t::elements + point_t::elements;

#pragma GCC diagnostic push
#if defined(COMPILER_GCC)
    #pragma GCC diagnostic ignored "-pedantic"
#endif
#if defined(COMPILER_CLANG)
    #pragma GCC diagnostic ignored "-Wgnu"
#endif
        union {
            struct {
                point_t  p;
                extent_t e;
            };
            struct {
                T x, y;
                T w, h;
            };
        };
#pragma GCC diagnostic pop

        //---------------------------------------------------------------------
        region () = default;
        region (extent_t);
        region (point_t, extent_t);
        region (point_t, point_t);
        region (std::array<T,S*2>);

        //---------------------------------------------------------------------
        size_type area     (void) const;
        size_type diameter (void) const;
        extent_t magnitude (void) const;
        extent_t magnitude (extent_t);

        void scale (T factor);

        bool empty (void) const;

        //---------------------------------------------------------------------
        point_t base    (void) const;
        point_t away    (void) const;
        point_t centre  (void) const;
        point_t closest (point_t) const;

        //---------------------------------------------------------------------
        // Point and region relation queries
        bool includes (point_t) const; // inclusive of borders
        bool contains (point_t) const; // exclusive of borders
        bool intersects (region<S,T>) const;  // exclusive of borders

        // Move a point to be within the region bounds
        void constrain (point_t&) const;
        point_t constrained (point_t) const;

        // Compute binary region combinations
        region intersection (region<S,T>) const;

        //---------------------------------------------------------------------
        region& resize (extent<S,T>);

        // Compute a region `mag` units into the region
        region inset (T mag);

        region expanded (T mag) const;
        region expanded (vector<S,T>) const;

        region& expand (T mag);
        region& expand (vector<S,T>);

        // arithmetic operators
        region operator+ (vector<S,T>) const;
        region operator- (vector<S,T>) const;

        // Logical comparison operators
        bool operator ==(region<S,T> rhs) const;
        bool operator !=(region<S,T> rhs) const
            { return !(*this == rhs); }

        // Utility constants
        static const region<S,T> MAX;
        static const region<S,T> UNIT;

        void sanity (void) const;
    };

    typedef region<2,size_t>   region2u;
    typedef region<2,intmax_t> region2i;
    typedef region<2,float>    region2f;
    typedef region<2,double>   region2d;

    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, const util::region<S,T>&);
}

#endif
