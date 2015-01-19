/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COORD_HPP
#define __UTIL_COORD_HPP

#include "../platform.hpp"

#include <algorithm>
#include <iterator>

#include <cstdlib>

namespace util {
    namespace detail {
        // Disable GCC warnings about validity of anonyous structures in
        // unions. Push comes to shove I'll manually redsign everything to
        // keep this syntax anyway.
#pragma GCC diagnostic push
#if defined(COMPILER_GCC)
    #pragma GCC diagnostic ignored "-pedantic"
#endif
#if defined(COMPILER_CLANG)
    #pragma GCC diagnostic ignored "-Wgnu"
#endif

        //---------------------------------------------------------------------
        // coord types are not really intended to have arbitrary dimension, so
        // don't add specialisations (or a general case) without a decent
        // reason.
        template <size_t S, typename T>
        struct coord_data;

        //---------------------------------------------------------------------
        template <typename T>
        struct coord_data<1, T>
        {
            coord_data () = default;
            coord_data (T v0):
                data { v0 }
            { ; }

            union {
                T data[1];
                T x;
            };
        };


        //---------------------------------------------------------------------
        template <typename T>
        struct coord_data<2,T>
        {
            coord_data () = default;
            coord_data (T v0, T v1):
                data { v0, v1 }
            { ; }

            union {
                T data[2];

                struct { T x, y; };
                struct { T s, t; };
            };
        };


        //---------------------------------------------------------------------
        template <typename T>
        struct coord_data<3,T>
        {
            coord_data () = default;
            coord_data (T v0, T v1, T v2):
                data { v0, v1, v2 }
            { ; }

            union {
                T data[3];

                struct { T x, y, z; };
                struct { T s, t, p; };
                struct { T r, g, b; };
            };
        };


        //---------------------------------------------------------------------
        template <typename T>
        struct coord_data<4,T>
        {
            coord_data () = default;
            coord_data (T v0, T v1, T v2, T v3):
                data { v0, v1, v2, v3 }
            { ; }

            union {
                T data[4];

                struct { T x, y, z, w; };
                struct { T s, t, p, q; };
                struct { T r, g, b, a; };
            };
        };


        //---------------------------------------------------------------------
        template <size_t S, typename T>
        struct coord : public coord_data<S,T> {
            static_assert (S > 0, "coord dimensions must be strictly positive");

            typedef T value_type;
            static constexpr size_t dimension = S;

            using coord_data<S,T>::coord_data;
            coord () = default;

            explicit coord (T v)
            { std::fill (std::begin (this->data), std::end (this->data), v); }

            coord (const coord<S,T> &rhs) = default;
            coord& operator= (const coord<S,T> &rhs) = default;

            T& operator[] (size_t i)       { return this->data[i]; }
            T  operator[] (size_t i) const { return this->data[i]; }
        };


        //---------------------------------------------------------------------
        // XXX: Unsure whether this should really be defined for arbitrary
        // types in a semantic sense, but practicality suggestes this is the
        // best option; point/vector dot product is too useful.
        template <size_t S, typename T>
        T dot (const coord<S,T> &a, const coord<S,T> &b)
        {
            return std::inner_product (std::begin (a.data),
                                       std::end   (a.data),
                                       std::begin (b.data),
                                       T {0});
        }
#pragma GCC diagnostic pop
    }
}

#endif
