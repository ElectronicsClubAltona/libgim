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
 * Copyright 2011-2013 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_SIGNAL_HPP
#error
#endif

namespace util {
    template <typename Ret, typename ...Args>
    signal<Ret, Args...>::scoped_cookie::scoped_cookie (cookie _cookie,
                   signal<Ret, Args...> &_parent):
        m_cookie (_cookie),
        m_parent (_parent)
    { ; }


    template <typename Ret, typename ...Args>
    signal<Ret, Args...>::scoped_cookie::scoped_cookie (scoped_cookie &&rhs):
        m_cookie (rhs.m_cookie),
        m_parent (rhs.m_parent)
    {
        rhs.m_cookie = rhs.m_parent.m_children.end ();
    }


    template <typename Ret, typename ...Args>
    signal<Ret, Args...>::scoped_cookie::~scoped_cookie () {
        if (m_parent.m_children.end () != m_cookie)
            m_parent.disconnect (m_cookie);
    }


    template <typename Ret, typename ...Args>
    void
    signal<Ret, Args...>::scoped_cookie::renew (callback_object &&cb)
        { *m_cookie = std::move (cb); }


    template <typename Ret, typename ...Args>
    void
    signal<Ret, Args...>::scoped_cookie::release (void)
        { m_cookie = m_parent.m_children.end (); }


    template <typename Ret, typename ...Args>
    signal<Ret, Args...>::signal ()
    { ; }


    template <typename Ret, typename ...Args>
    typename signal<Ret, Args...>::cookie
    signal<Ret, Args...>::connect (const callback_object &_cb)
        { return m_children.insert (m_children.end (), _cb); }


    template <typename Ret, typename ...Args>
    typename signal<Ret, Args...>::scoped_cookie
    signal<Ret, Args...>::scoped_connect (const callback_object &_cb)
        { return scoped_cookie (connect (_cb), *this); }


    /// Add a callback to the list.
    //const cookie
    //connect (const callback_function &_cb)
    //    { return m_children.insert (m_children.end (), _cb); }


    template <typename Ret, typename ...Args>
    void
    signal<Ret, Args...>::disconnect (const cookie _cb)
        { m_children.erase (_cb); }


    /// Disconnect all callbacks
    template <typename Ret, typename ...Args>
    void
    signal<Ret, Args...>::clear (void) 
        { m_children.clear (); }


    /// Returns the number of callbacks connected.
    template <typename Ret, typename ...Args>
    unsigned int
    signal<Ret, Args...>::size (void) const
        { return m_children.size (); }


    template <typename Ret, typename ...Args>
    bool
    signal<Ret, Args...>::empty (void) const
        { return m_children.empty (); }


    template <typename Ret, typename ...Args>
    void
    signal<Ret, Args...>::operator () (Args... tail) {
        if (m_children.empty ())
            return;

        auto i = m_children.cbegin ();
        bool looping;

        do {
            // Increment before we execute so that the caller is able to deregister during execution.
            auto current = i++;
            looping = m_children.cend () != i;

            (*current)(tail...);
        } while (looping);
    }
}

