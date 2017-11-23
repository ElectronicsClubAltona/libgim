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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_SIGNAL_HPP
#define __UTIL_SIGNAL_HPP

#include "types/traits.hpp"

#include <functional>
#include <list>

namespace util {
    namespace combine {
        template <typename F>
        struct logical_and {
            using R = typename func_traits<F>::return_type;

            template <typename T, typename ...Args>
            R operator() (T first, T last, Args&&... args)
            {
                while (first != last)
                    if (!(*first++)(args...))
                        return false;

                return true;
            }
        };

        template <typename F>
        struct logical_or {
            using R = typename func_traits<F>::return_type;

            template <typename T, typename ...Args>
            R operator() (T first, T last, Args&&... args)
            {
                while (first != last)
                    if ((*first++)(args...))
                        return true;

                return false;
            }
        };

        template <typename F>
        struct noop {
            using R = void;

            template <typename T, typename ...Args>
            R operator() (T first, T last, Args&&... args)
            {
                while (first != last) {
                    (*first++)(args...);
                }
            }
        };
    }

    template <typename F, template <typename> class C = combine::noop>
    class signal {
        public:
            using R = typename C<F>::R;
            using callback = std::function<F>;

            struct cookie;

        public:
            signal ();
            ~signal ();

            /// Add a callback to list.
            cookie connect [[gnu::warn_unused_result]] (callback&&);
            cookie connect [[gnu::warn_unused_result]] (const callback&);

            void disconnect (cookie&);

            /// Disconnect all callbacks
            void clear (void);

            /// Returns the number of callbacks connected.
            size_t size (void) const;
            bool empty (void) const;

            /// Execute all callbacks
            template <typename ...Args>
            R
            operator() (Args&&... tail);

        private:
            typedef std::list<callback> group;
            group m_children;
    };

    ///////////////////////////////////////////////////////////////////////////
    // wrap a value in a signal and trigger on assignment
    //template <typename T, template <typename> class C>
    template <typename T>
    class value_signal : public signal<void(T)> {
    public:
        explicit value_signal (T);
        value_signal () = default;

        operator const T&() const;

        value_signal<T>& operator= (const T&);

    private:
        T m_value;
    };
}

#include "signal.ipp"

#endif // __SIGNAL_HPP
