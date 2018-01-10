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

#include "iterator.hpp"

#include <cstdlib>
#include <stdexcept>


namespace util {
    /// An array with constant maximum size, but with actual used storage
    /// capacity fixed at construction time.
    ///
    /// \tparam S maximum number of elements
    /// \tparam T data type of elements
    template <std::size_t S, typename T>
    class sarray {
    public:
        template <typename InputIt>
        sarray (InputIt first, InputIt last):
            m_size (std::distance (first, last))
        {
            if (m_size > S)
                throw std::length_error ("oversize sarray");

            std::size_t i = 0;
            for (auto cursor = first; cursor != last; ++cursor)
                ::new (&m_data.objects[i++]) (T) (*cursor);
        }

        //---------------------------------------------------------------------
        template <typename ContainerT>
        explicit
        sarray (const ContainerT &_data):
            sarray (std::begin (_data), std::end (_data))
        { ; }

        //---------------------------------------------------------------------
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


    //-------------------------------------------------------------------------
    template <typename T, std::size_t S>
    auto
    make_sarray (const T(&data)[S], std::size_t count = S)
    {
        return sarray<S,T> (data, data + count);
    }
}

#endif
