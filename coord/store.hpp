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
 * Copyright 2015-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COORD_STORE_HPP
#define __UTIL_COORD_STORE_HPP

#include "fwd.hpp"

#include "../preprocessor.hpp"
#include "../platform.hpp"

#include <cstddef>
#include <type_traits>


///////////////////////////////////////////////////////////////////////////////
// Calculate a reasonable alignment for the given type and arity given what we
// know about the platform. Only intended to be used with alignas specifiers.
namespace util::coord::detail {
    template <typename T>
    constexpr
    size_t
    alignment (size_t S)
    {
        (void)S;

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


///////////////////////////////////////////////////////////////////////////////
// defines the named member variables that a coordinate type is composed of
#define DEFINE_COORD_STORE(TAG,...)             \
namespace util::coord {                         \
    template <typename T>                       \
    struct store<                               \
        VA_ARGS_COUNT(__VA_ARGS__),             \
        T,                                      \
        TAG                                     \
    > {                                         \
        union {                                 \
            struct {                            \
                T __VA_ARGS__;                  \
            };                                  \
            T data[VA_ARGS_COUNT(__VA_ARGS__)]; \
        };                                      \
    };                                          \
}


template <typename T>
struct util::coord::store<1,T,::util::extent<1,T>> {
    union { struct { T w; }; T data[1]; };
};

template <typename T>
struct util::coord::store<2,T,::util::extent<2,T>> {
    union { struct { T w, h; }; T data[2]; };
};

template <typename T>
struct util::coord::store<3,T,::util::extent<3,T>> {
    union { struct { T w, h, d; }; T data[3]; };
};

template <typename T>
struct util::coord::store<1,T,::util::point<1,T>> {
    union { struct { T x; }; T data[1]; };
};

template <typename T>
struct util::coord::store<2,T,::util::point<2,T>> {
    union { struct { T x, y; }; T data[2]; };
};

template <typename T>
struct util::coord::store<3,T,::util::point<3,T>> {
    union { struct { T x, y, z; }; T data[3]; };
};

template <typename T>
struct util::coord::store<4,T,::util::point<4,T>> {
    union { struct { T x, y, z, w; }; T data[4]; };
};

template <typename T>
struct util::coord::store<1,T,::util::vector<1,T>> {
    union { struct { T x; }; T data[1]; };
};

template <typename T>
struct util::coord::store<2,T,::util::vector<2,T>> {
    union { struct { T x, y; }; T data[2]; };
};

template <typename T>
struct util::coord::store<3,T,::util::vector<3,T>> {
    union { struct { T x, y, z; }; T data[3]; };
};

template <typename T>
struct util::coord::store<4,T,::util::vector<4,T>> {
    union { struct { T x, y, z, w; }; T data[4]; };
};

#endif
