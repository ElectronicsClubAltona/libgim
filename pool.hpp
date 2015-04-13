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
 * Copyright 2011-2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_POOL_HPP
#define __UTIL_POOL_HPP

#include "nocopy.hpp"

#include <cstdlib>

namespace util {
    template <typename T>
    class pool : public nocopy {
        protected:
            union node {
                char  _data[sizeof (T)];
                node *_chain;
            };

            node   *m_head;
            node   *m_next;
            size_t  m_capacity;

        public:
            pool (unsigned int _capacity);
            ~pool ();

            // Data management
            template <typename ...Args>
            T*   acquire (Args&... args);

            void release (T *data);

            size_t capacity (void) const;

            // Indexing
            size_t index (const T*) const;

            T& operator[] (size_t idx);
            const T& operator[] (size_t idx) const;
    };
}

#include "pool.ipp"

#endif // __UTIL_POOL_HPP
