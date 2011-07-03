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
 * Copyright 2011 Danny Robson <danny@blubinc.net>
 */

#ifndef __SIGNAL_HPP
#define __SIGNAL_HPP

#include <algorithm>
#include <vector>
#include <functional>


template <typename Ret, typename ...Args>
class signal {
    public:
        typedef Ret (*callback_function)(Args...);
        typedef std::function<Ret(Args...)> callback_object;

    protected:
        std::vector<callback_object> m_children;

    public:
        signal ()
            { m_children.reserve (16); }

        /// Add a callback to list.
        void connect (callback_object   _cb)
            { m_children.push_back (_cb); }

        /// Add a callback to the list.
        void connect (callback_function _cb)
            { m_children.push_back (_cb); }

        /// Remove all instances of callback `cb'
        //void disconnect (callback_function _cb)
        //    { disconnect (callback_object (_cb)); }

        /// Remove all instances of callback `cb'
        /*void disconnect (callback_object _cb) {
            m_children.erase (std::remove (m_children.begin (),
                                           m_children.end   (),
                                           _cb),
                              m_children.end ());
        }*/

        /// Disconnect all callbacks
        void clear (void)
            { m_children.clear (); }

        /// Returns the number of callbacks connected.
        unsigned int size (void) const
            { return m_children.size (); }

        /// Execute all callbacks, ignoring the return parameters. Does not combine results.
        void operator () (Args... tail) {
            for (auto i = m_children.begin (), end = m_children.end (); i != end; ++i)
                (*i)(tail...);
        }
};

#endif // __SIGNAL_HPP
