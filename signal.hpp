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
#define __UTIL_SIGNAL_HPP

#include "nocopy.hpp"

#include <functional>
#include <list>
#include <type_traits>

namespace util {
    template <typename F>
    class signal {
        public:
            using R = std::result_of<F>;
            using callback = std::function<F>;

            struct cookie;

        public:
            signal ();
            ~signal ();

            /// Add a callback to list.
            cookie connect (const callback&);

            void disconnect (const cookie&);

            /// Disconnect all callbacks
            void clear (void);

            /// Returns the number of callbacks connected.
            unsigned int size (void) const;
            bool empty (void) const;

            /// Execute all callbacks, ignoring the return parameters. Does
            /// not combine results.
            template <typename ...Args>
            void operator () (Args&&... tail);

        private:
            typedef std::list<callback> group;
            group m_children;
    };

    template <typename T>
    class value_signal : public signal<void(T)> {
    public:
        value_signal (T);
        value_signal () = default;

        operator const T&() const;

        value_signal<T>& operator= (const T&);

    private:
        T m_value;
    };
}

#include "signal.ipp"

#endif // __SIGNAL_HPP
