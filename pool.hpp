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
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */

#ifndef __UTIL_POOL_HPP
#define __UTIL_POOL_HPP

#include "nocopy.hpp"


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
        pool (unsigned int _capacity):
            m_capacity (_capacity)
        {
            static_assert (sizeof (T) >= sizeof (uintptr_t),
                           "pool<T>'s chained block system requires that T be at least pointer sized");

            m_head = (node *)operator new (sizeof (T) * m_capacity);
            m_next = m_head;

            for (unsigned int i = 0; i < m_capacity - 1; ++i)
                m_next[i]._chain = &m_next[i + 1];
            m_next[m_capacity - 1]._chain = NULL;
        }


        ~pool () {
            check (m_next != NULL);

            unsigned int doomed_count = 0;
            for (node *cursor = m_next; cursor != NULL; cursor = cursor->_chain)
                ++doomed_count;

            check_eq (doomed_count, m_capacity);
            operator delete (m_head);
        }
        

        unsigned int capacity (void) const
            { return m_capacity; }


        template <typename ...Args>
        T*   acquire (Args&... args) {
            if (!m_next)
                throw std::bad_alloc ();

            node *newnext = m_next->_chain;
            T *data = (T*)&m_next->_data;
            
            try {
                new (data) T (args...);
            } catch (...) {
                m_next->_chain = newnext;
                throw;
            }

            m_next  = newnext;
            return data;
        }


        void release (T *data) {
            data->~T();
            node *newnode = (node *)data;

            newnode->_chain = m_next;
            m_next = newnode;
        }
};


#endif // __UTIL_POOL_HPP
