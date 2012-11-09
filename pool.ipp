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

#ifdef __UTIL_POOL_IPP
#error Double inclusion of pool.ipp
#endif

#define __UTIL_POOL_IPP


namespace util {
    template <typename T>
    pool<T>::pool (unsigned int _capacity):
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


    template <typename T>
    pool<T>::~pool () {
        CHECK (m_next != NULL);

        unsigned int doomed_count = 0;
        for (node *cursor = m_next; cursor != NULL; cursor = cursor->_chain)
            ++doomed_count;

        CHECK_EQ (doomed_count, m_capacity);
        operator delete (m_head);
    }
            

    template <typename T>
    unsigned int
    pool<T>::capacity (void) const
        { return m_capacity; }


    template <typename T>
    template <typename ...Args>
    T*
    pool<T>::acquire (Args&... args) {
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


    template <typename T>
    void
    pool<T>::release (T *data) {
        data->~T();
        node *newnode = (node *)data;

        newnode->_chain = m_next;
        m_next = newnode;
    }
}


