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
#include "view.hpp"


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
    //
    template <typename IteratorT>
    struct numeric_iterator : public std::iterator<
        typename std::iterator_traits<IteratorT>::iterator_category,
        decltype (+std::declval<typename std::iterator_traits<IteratorT>::value_type> ()),
        typename std::iterator_traits<IteratorT>::difference_type,
        typename std::iterator_traits<IteratorT>::pointer,
        typename std::iterator_traits<IteratorT>::reference
    > {
        static_assert (std::is_arithmetic_v<typename std::iterator_traits<numeric_iterator>::value_type>);

        explicit numeric_iterator (IteratorT _inner):
            m_inner (_inner)
        { ; }

        auto operator++ (void) { ++m_inner; return *this; }

        auto
        operator- (const numeric_iterator &rhs) const
        {
            return typename std::iterator_traits<IteratorT>::difference_type { m_inner - rhs.m_inner };
        }

        auto
        operator* (void) const
        {
            return +*m_inner;
        }

        auto operator== (const numeric_iterator &rhs) const { return m_inner == rhs.m_inner; }
        auto operator!= (const numeric_iterator &rhs) const { return m_inner != rhs.m_inner; }

    private:
        IteratorT m_inner;
    };


    //-------------------------------------------------------------------------
    // convenience function that constructs a view of numeric_iterators for a
    // provided container
    template <typename ContainerT>
    auto
    numeric_view (ContainerT &data)
    {
        return util::view {
            numeric_iterator (std::begin (data)),
            numeric_iterator (std::end   (data))
        };
    }


    //-------------------------------------------------------------------------
    template <typename ContainerT>
    auto
    numeric_view (const ContainerT &data)
    {
        return util::view {
            numeric_iterator (std::begin (data)),
            numeric_iterator (std::end   (data))
        };
    }


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
        // holds a tuple of iterators for begin and end, and returns an
        // iterator that transforms these iterators into tuples of value_types.
        //
        // this must be expressed in terms of iterators, rather than containers,
        // because it dramatically simplifies iterating over raw arrays.
        template <
            typename IteratorT,
            typename I = std::make_index_sequence<std::tuple_size<IteratorT>::value>
        >
        class collection;


        //---------------------------------------------------------------------
        template <
            typename IteratorT,
            std::size_t ...I
        >
        class collection<
            IteratorT,
            std::index_sequence<I...>
        > {
        public:
            collection (const IteratorT &_begin, const IteratorT &_end):
                m_begin { _begin },
                m_end   { _end   }
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
            public:
                iterator (const IteratorT &_iterators):
                    m_iterators (_iterators)
                { ; }


                iterator&
                operator++ (void)
                {
                    // HACK: we don't actually need to create a tuple here,
                    // but it's a zero cost method to expand the parameter
                    // pack.
                    std::make_tuple (++std::get<I> (m_iterators)...);
                    return *this;
                }


                iterator operator++ (int);


                auto
                operator* (void)
                {
                    return std::make_tuple (*std::get<I> (m_iterators)...);
                }


                bool
                operator== (const iterator &rhs) const
                {
                    return m_iterators == rhs.m_iterators;
                }


                bool
                operator!= (const iterator &rhs) const
                {
                    return !(*this == rhs);
                }

            private:
                IteratorT m_iterators;
            };


            iterator
            begin (void)
            {
                return iterator { { std::get<I> (m_begin)... } };
            }


            iterator
            end (void)
            {
                return iterator { { std::get<I> (m_end)... } };
            }


        private:
            IteratorT m_begin;
            IteratorT m_end;
        };
    }


    ///------------------------------------------------------------------------
    /// takes a variable number of container arguments and returns an interable
    /// object with a value_type of tuple of the argument's value_types.
    ///
    /// the returned iterator value_type is suitable for using in range-for
    /// and structured bindings (and really, that's the entire point here).
    ///
    /// eg, util::zip ({1,2,3}, {4,5,6}) ~= {{1,4},{2,5},{3,6}}
    template <typename ...ContainerT>
    auto
    zip (const ContainerT&... data)
    {
        using IteratorT = std::tuple<decltype(std::begin(data))...>;

        return detail::zip::collection<
            IteratorT,
            std::make_index_sequence<sizeof...(ContainerT)>
        > {
            std::make_tuple (std::begin (data)...),
            std::make_tuple (std::end   (data)...)
        };
    };


    ///------------------------------------------------------------------------
    /// takes a variable number of containers and returns a zipped iterable
    /// object where the first of the iterator's value_types is the index of
    /// that iterator. ie, it combines container offsets with value_types.
    ///
    /// eg, util::izip ("abc") ~= {{0,'a'},{1,'b'},{2,'c'}}
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
