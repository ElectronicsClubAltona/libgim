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
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_SARRAY_HPP
#define CRUFT_UTIL_SARRAY_HPP

#include "./iterator.hpp"

#include <cstdlib>

namespace util {
    /// An array with constant maximum size, but with actual used storage
    /// capacity fixed at construction time.
    ///
    /// \tparam S maximum number of elements
    /// \tparam T data type of elements
    template <std::size_t S, typename T>
    class sarray {
    public:
        //---------------------------------------------------------------------
        template <typename ContainerT>
        explicit
        sarray (const ContainerT &_data):
            m_size (std::size (_data))
        {
            if (m_size > S)
                throw std::length_error("exceeded maximum size");

            // I'd like to use izip and structured bindings here, but we run
            // afoul of clang#3349 so we use a seperate index variable.
            std::size_t i = 0;
            for (auto &d: _data) {
                ::new (&m_data.objects[i++]) (T) (d);
            }
        }

        ~sarray ()
        {
            for (std::size_t i = 0; i < m_size; ++i)
                m_data.objects[i].~T ();
        }

        //---------------------------------------------------------------------
        std::size_t size (void) const
        {
            return m_size;
        }

        //---------------------------------------------------------------------
        T& operator[] (std::size_t i)& { return m_data.objects[i]; }
        const T& operator[] (std::size_t i) const& { return m_data.objects[i]; }

        auto begin (void) { return std::begin (m_data.objects); };
        auto end   (void) { return begin () + m_size; }

        auto begin (void) const { return std::begin (m_data.objects); };
        auto end   (void) const { return begin () + m_size; }

        auto cbegin (void) const { return std::cbegin (m_data.objects); };
        auto cend   (void) const { return cbegin () + m_size; }

    private:
        union alignas (T) data_t {
            // we manually define a trivial constructor/destructor to appease
            // gcc which doesn't like us using non-trivial T as the stored
            // object.
            //
            // we're explicitly controlling the lifetime ourselves so this
            // isn't a functional issue.
             data_t () { };
            ~data_t () { };

            char store[sizeof (T) * S];
            T objects[S];
        } m_data;

        const std::size_t m_size;
    };
}

#endif
