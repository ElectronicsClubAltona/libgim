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
    ///////////////////////////////////////////////////////////////////////////
    // Coordinate storage class.
    //
    // Types of arity multiples of 4 with payloads of at least 16 bytes
    // (T >= uint16_t) are guaranteed to be aligned appropriately for SSE (see
    // specialisations below).
    template <
        size_t S,
        typename T,
        typename...
    >
    struct
    alignas (
        sizeof (T) * S % 16 == 0 && sizeof (T) * S >= 16 ? 16 : alignof (T)
    )
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
    alignas (
        sizeof (T) * 4u >= 16 ? 16 : alignof (T)
    )
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
    alignas (
        sizeof (T) * 4u >= 16 ? 16 : alignof (T)
    )
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
    alignas (
        sizeof (T) * 4u >= 16 ? 16 : alignof (T)
    )
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
    struct store<4,T,wxyz,abcd> {
        union {
            T data[4];
            struct { T w,x,y,z; };
            struct { T a,b,c,d; };
        };
    };
} }

#endif
