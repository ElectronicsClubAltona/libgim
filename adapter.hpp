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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_ADAPTER_HPP
#define __UTIL_ADAPTER_HPP

#include <array>
#include <cstddef>
#include <iterator>

namespace util::adapter {
    namespace container {
        // reverse a container for range-based-for
        template <typename T>
        struct reverse {
            explicit reverse (T &_target):
                m_target (_target)
            { ; }

            auto begin (void) { return m_target.begin (); }
            auto end   (void) { return m_target.end (); }

            auto begin (void) const { return m_target.begin (); }
            auto end   (void) const { return m_target.end (); }

            auto cbegin (void) { return m_target.cbegin (); }
            auto cend   (void) { return m_target.cend (); }

        private:
            T &m_target;
        };


        // adapt a container's range methods to return indices rather than iterators
        template <typename T>
        struct indices {
            using typename T::size_type;

            explicit indices (T &_target):
                m_target (_target)
            { ; }

            size_type begin (void) { return 0; }
            size_type end   (void) { return m_target.size (); }

        private:
            T m_target;
        };
    }

    namespace iterator {
        // adapt an iterator to return the n-th tuple element of the
        // underlying iterator::value_type
        template <size_t I, typename It>
        struct scalar : public std::iterator<
            typename std::iterator_traits<It>::iterator_category,
            typename std::tuple_element<
                I,
                typename std::iterator_traits<
                    It
                >::value_type
            >::type,
            typename std::iterator_traits<It>::difference_type
        > {
        public:
            using reference  = typename std::iterator_traits<scalar<I,It>>::reference;
            using value_type = typename std::iterator_traits<scalar<I,It>>::value_type;

            explicit scalar (It _inner):
                m_inner (_inner)
            { ; }

            const reference operator* (void) const
            { return std::get<I> (*m_inner); }

            reference operator* (void)
            { return std::get<I> (*m_inner); }

            bool operator== (scalar<I,It> rhs) { return m_inner == rhs.m_inner; }
            bool operator!= (scalar<I,It> rhs) { return m_inner != rhs.m_inner; }

            scalar<I,It>& operator++ (void)
            { ++m_inner; return *this; }

        private:
            It m_inner;
        };


        //template <
        //    typename It,
        //    size_t S,
        //    typename T
        //>
        //struct explode : public std::enable_if<
        //    std::is_same<
        //        typename std::iterator_traits<It>::value_type,
        //        std::array<T,S>
        //    >::value,
        //    typename std::iterator<
        //        typename std::iterator_traits<It>::iterator_category,
        //        typename std::tuple_element<
        //            typename std::iterator_traits<It>::value_type
        //        >::type,
        //        typename std::iterator_traits<It>::difference_type
        //    >::type
        //> {
        //public:
        //    explode (It _inner):
        //        m_inner (_inner),
        //        m_sequence (0)
        //    { ; }

        //    const reference operator* (void) const
        //    { return (*m_inner)[m_seqence]; }

        //    reference operator* (void)
        //    { return (*m_inner)[m_seqence]; }

        //    bool operator== (explode rhs) { return m_inner == rhs.m_inner && m_sequence == rhs.m_sequence; }
        //    bool operator!= (explode rhs) { return !(*this == rhs); }

        //    explode<It>& operator++ (void)
        //    {
        //        if (++m_sequence >= S)
        //            ++m_inner;
        //    }

        //private:
        //    It m_inner;
        //    size_t m_sequence;
        //};
    }
}

#endif
