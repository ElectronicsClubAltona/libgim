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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __POINTER_HPP
#define __POINTER_HPP

#include <cstddef>
#include <cstdint>


namespace util {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    T*
    align (T *_ptr, size_t alignment)
    {
        auto ptr = reinterpret_cast<uintptr_t> (_ptr);
        if (ptr % alignment)
            ptr += alignment - ptr % alignment;
        return reinterpret_cast<T*> (ptr);
    }
}

#endif
