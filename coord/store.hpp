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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COORD_STORE_HPP
#define __UTIL_COORD_STORE_HPP

#include "names.hpp"

#include "../platform.hpp"

#include <cstdlib>

namespace util { namespace coord {
    ///////////////////////////////////////////////////////////////////////
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

    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T, typename...>
    struct store {
        T data[S];
    };


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct store<3,T,rgba,hsv> {
        union {
            T data[3];
            struct { T r,g,b; };
            struct { T h,s,v; };
        };
    };

    template <typename T>
    struct store<4,T,rgba,hsv> {
        union {
            T data[4];
            struct { T r,g,b,a; };
            struct { T h,s,v;   };
        };
    };


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct store<2,T,xyzw> {
        union {
            T data[2];
            struct { T x,y; };
        };
    };


    template <typename T>
    struct store<3,T,xyzw> {
        union {
            T data[3];
            struct { T x,y,z; };
        };
    };


    template <typename T>
    struct store<4,T,xyzw> {
        union {
            T data[4];
            struct { T x,y,z,w; };
        };
    };


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct store<2,T,xyzw,stpq> {
        union {
            T data[2];
            struct { T x,y; };
            struct { T s,t; };
        };
    };


    template <typename T>
    struct store<3,T,xyzw,stpq> {
        union {
            T data[3];
            struct { T x,y,z; };
            struct { T s,t,p; };
        };
    };


    template <typename T>
    struct store<4,T,xyzw,stpq> {
        union {
            T data[4];
            struct { T x,y,z,w; };
            struct { T s,t,p,q; };
        };
    };


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct store<2,T,whd> {
        union {
            T data[2];
            struct { T w,h; };
        };
    };


    template <typename T>
    struct store<3,T,whd> {
        union {
            T data[3];
            struct { T w,h,d; };
        };
    };
} }

#endif
