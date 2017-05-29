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
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_TYPEIDX_HPP
#define CRUFT_UTIL_TYPEIDX_HPP

#include <cstdint>

namespace util {
    namespace detail { std::uintptr_t typeidx_next (void); }

    /// return a globally unique runtime ID for a given type.
    ///
    /// this is intended to be used as a lightweight type check for variants
    /// and such without requiring RTTI.
    ///
    /// the identifier is constructed at runtime and is not guaranteed to be
    /// stable across executions (particularly in the case of threads and
    /// other non-determinism).
    ///
    /// the range of identifiers is _probably_ contiguous starting from zero.
    /// this should not be relied upon for correctness, but may be used for
    /// performance concerns.
    template <typename T>
    std::uintptr_t
    typeidx (void)
    {
        static auto id = detail::typeidx_next ();
        return id;
    }
}

#endif
