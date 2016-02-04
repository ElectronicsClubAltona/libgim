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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_POOL_HPP
#define __UTIL_POOL_HPP

#include "nocopy.hpp"

#include <cstdlib>

namespace util {
    /// a simple pre-allocated pool for storage of PODs.
    ///
    /// non-POD types can be stored, but there are no guarantees for calling
    /// item destructors at pool destruction time.
    template <typename T>
    class pool : public nocopy {
        protected:
            union node {
                node *_node;
                char  _data[sizeof (T)];
            };

            node *m_head; // root address of allocation
            node *m_next; // next available entry in the linked list

            const size_t m_capacity;
            size_t m_size;

        public:
            pool (unsigned int _capacity);
            ~pool ();

            // Data management
            template <typename ...Args>
            T*   acquire (Args&... args);

            void release (T *data);

            size_t capacity (void) const;
            size_t size (void) const;
            bool empty (void) const;

            // Indexing
            size_t index (const T*) const;

            T& operator[] (size_t idx);
            const T& operator[] (size_t idx) const;
    };
}

#include "pool.ipp"

#endif // __UTIL_POOL_HPP
