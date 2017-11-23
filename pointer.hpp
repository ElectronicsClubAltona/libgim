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

#ifndef CRUFT_UTIL_POINTER_HPP
#define CRUFT_UTIL_POINTER_HPP

#include <cstddef>
#include <cstdint>


namespace util {
    ///////////////////////////////////////////////////////////////////////////
    /// round the pointer upwards to satisfy the provided alignment
    template <typename T>
    constexpr T*
    align (T *_ptr, size_t alignment)
    {
        // we perform this as two steps to avoid unnecessarily incrementing when
        // remainder is zero.
        auto ptr = reinterpret_cast<uintptr_t> (_ptr);
        if (ptr % alignment)
            ptr += alignment - ptr % alignment;
        return reinterpret_cast<T*> (ptr);
    }


    ///------------------------------------------------------------------------
    /// round the pointer upwards to satisfy the provided alignment
    constexpr inline uintptr_t
    align (uintptr_t ptr, size_t alignment)
    {
        // we perform this as two steps to avoid unnecessarily incrementing when
        // remainder is zero.
        if (ptr % alignment)
            ptr += alignment - ptr % alignment;
        return ptr;
    }


    ///------------------------------------------------------------------------
    /// round the pointer upwards to the nearest valid alignment for T
    template <typename T>
    constexpr auto
    align (T *t)
    {
        return align (t, alignof (T));
    }


    ///------------------------------------------------------------------------
    /// round the pointer upwards to the nearest valid alignment for T
    template <typename T>
    constexpr auto
    align (uintptr_t ptr)
    {
        return align (ptr, alignof (T));
    }
}

#endif
