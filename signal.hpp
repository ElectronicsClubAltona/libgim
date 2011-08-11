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

#include "debug.hpp"
#include "nocopy.hpp"

#include <algorithm>
#include <list>
#include <functional>

namespace util {
    template <typename Ret, typename ...Args>
    class signal {
        public:
            typedef Ret (*callback_function)(Args...);
            typedef std::function<Ret(Args...)> callback_object;

        protected:
            typedef std::list<callback_object> group;
            group m_children;

        public:
            typedef typename group::iterator cookie;
            struct scoped_cookie : public nocopy {
                cookie                m_cookie;
                signal<Ret, Args...>& m_parent;

                scoped_cookie (cookie _cookie,
                               signal<Ret, Args...> &_parent):
                    m_cookie (_cookie),
                    m_parent (_parent)
                { ; }

                scoped_cookie (scoped_cookie &&rhs):
                    m_cookie (rhs.m_cookie),
                    m_parent (rhs.m_parent)
                {
                    rhs.m_cookie = rhs.m_parent.m_children.end ();
                }

                ~scoped_cookie () {
                    if (m_parent.m_children.end () != m_cookie)
                        m_parent.disconnect (m_cookie);
                }

                void renew (callback_object &&cb)
                    { *m_cookie = std::move (cb); }
            };

        public:
            signal ()
                { /*m_children.reserve (16);*/ }


            /// Add a callback to list.
            const cookie
            connect (const callback_object &_cb)
                { return m_children.insert (m_children.end (), _cb); }


            /// Add a callback to the list.
            const cookie
            connect (const callback_function &_cb)
                { return m_children.insert (m_children.end (), _cb); }


            void disconnect (const cookie _cb)
                { m_children.erase (_cb); }


            /// Disconnect all callbacks
            void clear (void) 
                { m_children.clear (); }


            /// Returns the number of callbacks connected.
            unsigned int size (void) const
                { return m_children.size (); }


            bool empty (void) const
                { return m_children.empty (); }


            /// Execute all callbacks, ignoring the return parameters. Does not combine results.
            void operator () (Args... tail) {
                for (auto i = m_children.cbegin (); i != m_children.cend (); ++i)
                    (*i)(tail...);
            }
    };
}

#endif // __SIGNAL_HPP
