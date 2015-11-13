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

#include <memory>

namespace util { namespace alloc {
    template <class T>
    class arena {
    public:
        arena (T &store);

        template <typename U>
        U* acquire (void);

        template <typename U>
        std::unique_ptr<U> unique (void);

        template <typename U>
        void release (U*);

    private:
        T &m_store;
    };
} }

#include "./arena.hpp"

#endif
