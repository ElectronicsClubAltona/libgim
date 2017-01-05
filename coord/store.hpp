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
#include <type_traits>

// Calculate a reasonable alignment for the given type and arity given what we
// know about the platform. Only intended to be used with alignas specifiers.
namespace util::coord::detail {
    template <typename T>
    constexpr
    size_t
    alignment (size_t S)
    {
#if defined(__SSE_MATH__)
        // Align to 16 if we have 4x floats on SSE/NEON. There are other
        // possiblities, but we don't care about them right at this point.
        if (!std::is_same<T,float>::value)
            return alignof (T);

        if (S % 4 == 0)
            return 16;
#elif defined (__ARM_NEON__)
        // TODO: deal with alignment issues before adding alignas specifiers
#endif

        return alignof (T);
    }
}

#define SIMD_ALIGN(S,T) alignas (util::coord::detail::alignment<T> (S))


namespace util::coord {
    ///////////////////////////////////////////////////////////////////////////
    // Coordinate storage class.
    //
    // Types with trivially suitable arity are aligned appropriately to take
    // advantage of native platform SIMD. eg, 4f types are aligned to 16 bytes
    // on SSE platforms.
    template <
        size_t S,
        typename T,
        typename...
    >
    struct
    SIMD_ALIGN(S,T)
    store {
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


    // Align on 16 bytes if the data is at least 16 bytes long. Prevents tiny
    // types like colour4u requiring massive alignments, reduces internal
    // fragmentation.
    //
    // TODO: expand this for other instruction sets. maybe switch on type.
    template <typename T>
    struct
    SIMD_ALIGN(4,T)
    store<4,T,rgba,hsv> {
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


    // Align on 16 bytes if the data is at least 16 bytes long. Prevents tiny
    // types like colour4u requiring massive alignments, reduces internal
    // fragmentation.
    //
    // TODO: expand this for other instruction sets. maybe switch on type.
    template <typename T>
    struct
    SIMD_ALIGN(4,T)
    store<4,T,xyzw> {
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


    // Align on 16 bytes if the data is at least 16 bytes long. Prevents tiny
    // types like colour4u requiring massive alignments, reduces internal
    // fragmentation.
    //
    // TODO: expand this for other instruction sets. maybe switch on type.
    template <typename T>
    struct
    SIMD_ALIGN(4,T)
    store<4,T,xyzw,stpq> {
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


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct
    SIMD_ALIGN(4,T)
    store<4,T,wxyz,abcd> {
        union {
            T data[4];
            struct { T w,x,y,z; };
            struct { T a,b,c,d; };
        };
    };
}

#endif
