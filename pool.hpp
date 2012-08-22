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

namespace util {
    template <typename T>
    class pool : public nocopy {
        protected:
            union node {
                char  _data[sizeof (T)];
                node *_chain;
            };

            node         *m_head;
            node         *m_next;
            unsigned int  m_capacity;

        public:
            pool (unsigned int _capacity);
            ~pool ();

            unsigned int capacity (void) const;

            template <typename ...Args>
            T*   acquire (Args&... args);

            void release (T *data);
    };
}

#include "pool.ipp"

#endif // __UTIL_POOL_HPP
