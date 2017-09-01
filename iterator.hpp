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
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_ITERATOR_HPP
#define __UTIL_ITERATOR_HPP

#include "types/traits.hpp"
#include "variadic.hpp"


template <typename Base>
class referencing_iterator {
    protected:
        typedef typename std::enable_if<
            is_dereferencable<
                typename Base::value_type
            >::value,
            typename Base::value_type
        >::type base_value_type;

    public:
        typedef typename dereferenced_type<base_value_type>::type value_type        ;
        typedef typename Base::difference_type                    difference_type   ;
        typedef value_type&                                       reference         ;
        typedef value_type*                                       pointer;
        typedef typename Base::iterator_category                  iterator_category;

    protected:
        Base m_base;

    public:
        explicit referencing_iterator (Base _base):
            m_base (_base)
        { ; }

        referencing_iterator& operator++()    {                   ++m_base; return *this; }
        referencing_iterator  operator++(int) { auto val = *this; ++m_base; return   val; }

        bool operator== (const referencing_iterator<Base> &rhs) { return m_base == rhs.m_base; }
        bool operator!= (const referencing_iterator<Base> &rhs) { return m_base != rhs.m_base; }
        bool operator>= (const referencing_iterator<Base> &rhs) { return m_base >= rhs.m_base; }
        bool operator<= (const referencing_iterator<Base> &rhs) { return m_base <= rhs.m_base; }
        bool operator>  (const referencing_iterator<Base> &rhs) { return m_base >  rhs.m_base; }
        bool operator<  (const referencing_iterator<Base> &rhs) { return m_base <  rhs.m_base; }

        const value_type& operator*() const
            { return **m_base; }
        reference operator*()
            { return **m_base; }

        difference_type operator-(const referencing_iterator<Base>& rhs) const { return m_base - rhs.m_base; }
        referencing_iterator<Base> operator-(int rhs) const { return referencing_iterator (m_base - rhs); }
        referencing_iterator<Base> operator+(int rhs) const { return referencing_iterator (m_base + rhs); }

};


namespace util {
    ///////////////////////////////////////////////////////////////////////////
    /// an output iterator that inserts a delimiter between successive
    /// assignments
    ///
    /// very useful for outputting comma seperated lists to an ostream, eg:
    ///
    /// std::copy (
    ///     std::cbegin (container),
    ///     std::cend   (container),
    ///     util::infix_iterator<value_type> (os, ", ")
    /// );
    template <
        typename T,
        class CharT = char,
        class Traits = std::char_traits<CharT>
    >
    class infix_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {
    public:
        using char_type    = CharT;
        using traits_type  = Traits;
        using ostream_type = std::basic_ostream<char_type, traits_type>;

        infix_iterator (ostream_type& _output, const CharT *_delimiter):
            m_output (_output),
            m_delimiter (_delimiter)
        { ; }

        infix_iterator&
        operator= (const T &value)
        {
            if (!m_first)
                m_output << m_delimiter;

            m_output << value;
            m_first = false;

            return *this;
        }

        infix_iterator& operator*  (void) { return *this; }
        infix_iterator& operator++ (void) { return *this; }
        infix_iterator& operator++ (int)  { return *this; }

    private:
        bool m_first = true;
        ostream_type &m_output;
        const CharT *m_delimiter;
    };


    namespace detail {
        template <typename ContainerT, typename CharT>
        struct infix_t {
            const ContainerT &_container;
            const CharT *_delimiter;
        };

        template <typename ContainerT, typename CharT>
        std::ostream&
        operator<< (std::ostream &os, const infix_t<ContainerT,CharT> &val)
        {
            std::copy (std::cbegin (val._container),
                       std::cend (val._container),
                       infix_iterator<typename ContainerT::value_type> (os, val._delimiter));
            return os;
        }

    };


    /// a helper function that returns an object that will use a
    /// util::infix_iterator to output a container's values to an ostream with
    /// the given delimiter.
    ///
    /// reduces boilerplate code required to output lists of things
    template <typename ContainerT, typename CharT = char>
    auto
    make_infix (const ContainerT &_container, const CharT *_delimiter = ", ")
    {
        return detail::infix_t<ContainerT,CharT> { _container, _delimiter };
    };


    ///////////////////////////////////////////////////////////////////////////
    template <typename ContainerT>
    class indices {
    public:
        indices (const ContainerT &_container):
            m_container (_container)
        { ; }

        class iterator : public std::iterator<std::forward_iterator_tag, std::size_t, std::size_t> {
        public:

            iterator (std::size_t _index):
                m_index (_index)
            { ; }

            bool
            operator!= (const iterator &rhs) const
            {
                return m_index != rhs.m_index;
            }

            bool
            operator== (const iterator &rhs) const
            {
                return m_index == rhs.m_index;
            }

            iterator&
            operator++ (void) &
            {
                ++m_index;
                return *this;
            };

            const std::size_t&
            operator* (void) const&
            {
                return m_index;
            }

        private:
            std::size_t m_index;
        };

        iterator begin (void) const { return iterator { 0 }; }
        iterator end   (void) const { return iterator { m_container.size () }; }

    private:
        const ContainerT &m_container;
    };


    //-------------------------------------------------------------------------
    template <typename T>
    indices<T>
    make_indices (const T &_t)
    {
        return indices<T> (_t);
    }


    ///////////////////////////////////////////////////////////////////////////
    namespace detail::zip {
        template <
            typename ContainerT,
            typename IteratorT,
            typename I = std::make_index_sequence<std::tuple_size<ContainerT>::value>
        >
        struct collection;

        template <
            typename ContainerT,
            typename IteratorT,
            std::size_t ...I
        >
        struct collection<
            ContainerT,
            IteratorT,
            std::index_sequence<I...>
        > {
            collection (const ContainerT &_containers):
                m_containers { _containers }
            { ; }

            struct iterator : std::iterator<
                std::forward_iterator_tag,
                std::tuple<
                    typename std::iterator_traits<
                        typename std::tuple_element<I,IteratorT>::type
                    >::value_type...
                >,
                std::size_t
            > {
                IteratorT m_iterators;

                iterator (IteratorT _iterators):
                    m_iterators (_iterators)
                { ; }

                iterator& operator++ (void)
                {
                    // HACK: we don't actually need to create a tuple here,
                    // but it's a zero cost method to expand the parameter
                    // pack.
                    std::make_tuple (++std::get<I> (m_iterators)...);
                    return *this;
                }

                iterator operator++ (int);

                auto operator* (void)
                {
                    return std::make_tuple (*std::get<I> (m_iterators)...);
                }

                bool operator== (const iterator &rhs) const
                {
                    return m_iterators == rhs.m_iterators;
                }

                bool operator!= (const iterator &rhs) const
                {
                    return !(*this == rhs);
                }
            };

            iterator begin (void)
            {
                return iterator { { std::begin (std::get<I> (m_containers))... } };
            }

            iterator end (void)
            {
                return iterator { { std::end (std::get<I> (m_containers))... } };
            }

            ContainerT m_containers;
        };
    }


    //-------------------------------------------------------------------------
    template <typename ...ContainerT>
    auto
    zip (const ContainerT&... data)
    {
        using container = std::tuple<ContainerT...>;
        using iterator = std::tuple<decltype(std::begin(data))...>;

        return detail::zip::collection<
            container,
            iterator,
            std::make_index_sequence<sizeof...(ContainerT)>
        > (
            std::make_tuple (data...)
        );
    };


    //-------------------------------------------------------------------------
    template <typename ...ContainerT>
    auto
    izip (const ContainerT&... data)
    {
        return zip (
            ::util::make_indices (::util::variadic::first (data...)),
            data...
        );
    }


    ///////////////////////////////////////////////////////////////////////////
    /// an output iterator that always discards any parameters on assignment.
    ///
    /// sometimes useful to pass to algorithms that generate useful results as
    /// a return value, while not caring about the implicit OutputIterator
    /// results.
    struct discard_iterator : public std::iterator<std::output_iterator_tag, discard_iterator> {
        template <typename T>
        void operator= (const T&) { ; }

        discard_iterator& operator++ (   ) { return *this; }
        discard_iterator  operator++ (int) { return *this; }
        discard_iterator& operator*  (   ) { return *this; }
    };
};

#endif
