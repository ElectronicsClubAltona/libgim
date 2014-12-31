/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
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
