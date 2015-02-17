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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_SIGNAL_HPP
#error
#endif

#include "debug.hpp"

#include <algorithm>

namespace util {
    ///////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct signal<F>::cookie : public nocopy {
        cookie (typename group::iterator, signal<F> &parent);
        cookie (cookie &&rhs);
        ~cookie ();

        void reset (callback &&cb);
        void reset (void);

        typename group::iterator m_position;
        signal<F> &m_parent;
    };


    ///////////////////////////////////////////////////////////////////////////
    template <typename F>
    signal<F>::cookie::cookie (typename group::iterator _position,
                               signal<F> &_parent):
        m_position (_position),
        m_parent (_parent)
    { ; }


    //-------------------------------------------------------------------------
    template <typename F>
    signal<F>::cookie::cookie (cookie &&rhs):
        m_position (rhs.m_position),
        m_parent (rhs.m_parent)
    {
        rhs.m_position = rhs.m_parent.m_children.end ();
    }


    //-------------------------------------------------------------------------
    template <typename F>
    signal<F>::cookie::~cookie ()
    {
        if (m_parent.m_children.end () != m_position)
            m_parent.disconnect (*this);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename F>
    void
    signal<F>::cookie::reset (callback &&cb)
    {
        *m_position = std::move (cb);
    }


    //-------------------------------------------------------------------------
    template <typename F>
    void
    signal<F>::cookie::reset (void)
    {
        m_position = m_parent.m_children.end ();
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename F>
    signal<F>::signal ()
    { ; }


    //-------------------------------------------------------------------------
    template <typename F>
    signal<F>::~signal ()
    {
        CHECK (empty ());
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename F>
    typename signal<F>::cookie
    signal<F>::connect (const callback &_cb)
    {
        return cookie (m_children.insert (m_children.end (), _cb), *this);
    }


    //-------------------------------------------------------------------------
    template <typename F>
    void
    signal<F>::disconnect (const cookie &c)
    {
        m_children.erase (c.m_position);
    }


    //-------------------------------------------------------------------------
    /// Disconnect all callbacks
    template <typename F>
    void
    signal<F>::clear (void) 
    {
        m_children.clear ();
    }


    ///////////////////////////////////////////////////////////////////////////
    /// Returns the number of callbacks connected.
    template <typename F>
    unsigned int
    signal<F>::size (void) const
    {
        return m_children.size ();
    }


    //-------------------------------------------------------------------------
    template <typename F>
    bool
    signal<F>::empty (void) const
    {
        return m_children.empty ();
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename F>
    template <typename ...Args>
    void
    signal<F>::operator () (Args&&... tail) {
        if (m_children.empty ())
            return;

        auto i = m_children.cbegin ();
        bool looping;

        do {
            // Increment before we execute so that the caller is able to
            // deregister during execution.
            auto current = i++;
            looping = m_children.cend () != i;

            (*current)(std::forward<Args> (tail)...);
        } while (looping);
    }
}

