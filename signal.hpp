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
#define __UTIL_SIGNAL_HPP

#include "debug.hpp"
#include "nocopy.hpp"

#include <algorithm>
#include <list>
#include <functional>

namespace util {
    template <typename Ret, typename ...Args>
    class signal {
        public:
            //typedef Ret (*callback_function)(Args...);
            typedef std::function<Ret(Args...)> callback_object;

        protected:
            typedef std::list<callback_object> group;
            group m_children;

        public:
            typedef typename group::iterator cookie;
            struct scoped_cookie : public nocopy {
                cookie                m_cookie;
                signal<Ret, Args...>& m_parent;

                scoped_cookie (cookie _cookie, signal<Ret, Args...> &_parent);
                scoped_cookie (scoped_cookie &&rhs);
                ~scoped_cookie ();

                void renew (callback_object &&cb);
                void release (void);
            };

        public:
            signal ();

            /// Add a callback to list.
            cookie connect (const callback_object &_cb);
            scoped_cookie scoped_connect (const callback_object &_cb);

            void disconnect (const cookie _cb);
            /// Disconnect all callbacks
            void clear (void);

            /// Returns the number of callbacks connected.
            unsigned int size (void) const;
            bool empty (void) const;

            /// Execute all callbacks, ignoring the return parameters. Does not combine results.
            void operator () (Args... tail);
    };
}

#include "signal.ipp"

#endif // __SIGNAL_HPP
