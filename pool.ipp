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

#ifdef __UTIL_POOL_IPP
#error
#endif

#define __UTIL_POOL_IPP

#include "debug.hpp"

#include <cstdint>
#include <new>
#include <string>


namespace util {
    //-------------------------------------------------------------------------
    template <typename T>
    pool<T>::pool (unsigned int _capacity):
        m_capacity (_capacity),
        m_size     (0u)
    {
        static_assert (sizeof (T) >= sizeof (uintptr_t),
                       "pool<T>'s chained block system requires that T be at least pointer sized");

        // allocate the memory and note the base address for deletion in destructor
        m_next = m_head = new node[m_capacity]; // static_cast<node *> (operator new (sizeof (T) * m_capacity));

        // initialise the linked list of nodes
        for (unsigned int i = 0; i < m_capacity - 1; ++i)
            m_next[i]._node = m_next + i + 1;
        m_next[m_capacity - 1]._node = nullptr;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    pool<T>::~pool ()
    {
        // don't check if everything's been returned as pools are often used
        // for PODs which don't need to be destructed via calling release.
        delete [] m_head;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    size_t
    pool<T>::capacity (void) const
    {
        return m_capacity;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    size_t
    pool<T>::size (void) const
    {
        return m_size;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    bool
    pool<T>::empty (void) const
    {
        return m_size == m_capacity;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    template <typename ...Args>
    T*
    pool<T>::acquire (Args&... args)
    {
        // double check we have enough capacity left
        if (!m_next)
            throw std::bad_alloc ();
        CHECK_LT (m_size, m_capacity);

        // save what will become the next node shortly. it could be overwritten
        // in the constructor we're about to call.
        node *newnext = m_next->_node;
        T *data = reinterpret_cast<T*> (m_next);

        // try to construct the returnable object.
        try {
            new (data) T (args...);
        } catch (...) {
            // the constructor may have overwritten the node linkages before
            // throwing. fix this up before forwarding the exception.
            m_next->_node = newnext;
            throw;
        }

        // the object is valid. save the new linked list head and bump the
        // stats for availability.
        m_next  = newnext;
        m_size++;

        return data;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    void
    pool<T>::release (T *data)
    {
        CHECK_NEZ (m_size);

        data->~T();
        node *newnode = reinterpret_cast<node *> (data);

        newnode->_node = m_next;
        m_next = newnode;
        m_size--;
    }
}


