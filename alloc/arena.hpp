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

#ifndef __UTIL_ALLOC_ARENA_HPP
#define __UTIL_ALLOC_ARENA_HPP

#include "../memory/deleter.hpp"

#include <memory>
#include <utility>

namespace util::alloc {
    template <class T>
    class arena {
    public:
        explicit arena (T &store);

        //---------------------------------------------------------------------
        template <typename U, typename ...Args>
        U*
        acquire (Args&&...);

        //---------------------------------------------------------------------
        template <typename U>
        void
        release (U*);

        //---------------------------------------------------------------------
        template <typename U>
        using deleter_t = util::memory::owner_deleter<
            U,arena<T>,&arena::release
        >;

        template <typename U>
        using unique_t = std::unique_ptr<U,deleter_t<U>>;

        // the return type must be auto and the implementation must be inline
        // otherwise we trigger an internal compiler error in gcc-5.2.0
        // "sorry, unimplemented: mangling offset_ref"
        template <typename U, typename ...Args>
        auto
        unique (Args&& ...args)
        {
            return unique_t<U> {
                acquire<U> (std::forward<Args> (args)...),
                deleter_t<U> (*this)
            };
        }


    private:
        T &m_store;
    };
}

#include "arena.ipp"

#endif
