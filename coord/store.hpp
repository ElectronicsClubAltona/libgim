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

#pragma GCC diagnostic pop
} }

#endif
