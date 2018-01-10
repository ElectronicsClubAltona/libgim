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
 * Copyright 2015-2018 Danny Robson <danny@nerdcruft.net>
 */


#ifndef CRUFT_UTIL_VIEW_HPP
#define CRUFT_UTIL_VIEW_HPP

#include "types/traits.hpp"
#include "maths.hpp"

#include <cstdlib>
#include <ostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <iterator>

namespace util {
    template <typename BeginT, typename EndT = BeginT>
    struct view {
    public:
        //---------------------------------------------------------------------
        using value_type = typename std::iterator_traits<
            remove_restrict_t<BeginT>
        >::value_type;


        //---------------------------------------------------------------------
        constexpr
        view (const BeginT &first, const EndT &last) noexcept:
            m_begin (first),
            m_end   (last)
        { ; }


        //---------------------------------------------------------------------
        // cosntruction from pointer/size represenations for ease of use with
        // legacy C code.
        template <
            typename CountT,
            typename = std::enable_if_t<std::is_integral_v<CountT>,void>
        >
        view (
            const BeginT &_begin,
            CountT _size
        ):
            view (_begin, _begin + _size)
        { ; }


        //---------------------------------------------------------------------
        // implicit conversion from pointer views to const pointer views
        template <
            typename ValueT,
            typename = std::enable_if_t<
                std::is_same_v<BeginT, const ValueT*> &&
                std::is_same_v<EndT,   const ValueT*>
            >
        >
        view (const view<ValueT*,ValueT*> &rhs):
            m_begin (rhs.begin ()),
            m_end   (rhs.end   ())
        { ; }


        //---------------------------------------------------------------------
        // explicitly cater for the char array case so that we don't
        // accidentally include the trailing null in the data.
        template <std::size_t N>
        view (const char (&value)[N]):
            view {std::begin (value), std::begin (value) + N - 1}
        {
            static_assert (N > 0);
        }


        //---------------------------------------------------------------------
        view (const char *str):
            view { str, str + strlen (str) }
        { ; }


        //---------------------------------------------------------------------
        view (char *str):
            view (str, str + strlen (str))
        { ; }


        //---------------------------------------------------------------------
        template <std::size_t N>
        view (char (&value)[N]):
            view {std::begin (value), std::begin (value) + N - 1}
        {
            static_assert (N > 0);
        }


        //---------------------------------------------------------------------
        template <std::size_t N, typename ValueT>
        view (const ValueT(&value)[N]):
            view {std::begin (value), std::end (value)}
        { ; }


        //---------------------------------------------------------------------
        template <std::size_t N, typename ValueT>
        view (ValueT(&value)[N]):
            view {std::begin (value), std::end (value)}
        { ; }


        //---------------------------------------------------------------------
        constexpr
        view (const view &rhs) noexcept:
            view {rhs.m_begin, rhs.m_end}
        { ; }


        //---------------------------------------------------------------------
        // technically we could get away without explicitly defining a move
        // constructor here, but by nulling rhs we can more easily use this
        // class as a base for unique owning pointers without exposing the
        // begin/end data members to them directly.
        constexpr view (view &&rhs) noexcept:
            view {std::move (rhs.m_begin), std::move (rhs.m_end)}
        { ; }


        //---------------------------------------------------------------------
        // allow null construction of views where IteratorT is constructible
        // from nullptr_t
        //
        // ideally we would avoid exposing this as it promotes use of nulls but
        // it simplifies construction of views that are data members of classes
        // when we may not immediately know the values we should contain.
        constexpr view (std::nullptr_t) noexcept:
            view {nullptr,nullptr}
        { ; }


        //---------------------------------------------------------------------
        template <typename CharT, typename Traits, typename Allocator>
        view (std::basic_string<CharT,Traits,Allocator> &val):
            view (std::data (val), std::data (val) + std::size (val))
        { ; }


        //---------------------------------------------------------------------
        template <typename CharT, typename Traits, typename Allocator>
        view (const std::basic_string<CharT,Traits,Allocator> &val):
            view (std::data (val), std::data (val) + std::size (val))
        { ; }


        //---------------------------------------------------------------------
        template <typename ValueT, typename AllocatorT>
        view (const std::vector<ValueT,AllocatorT> &rhs):
            view (std::data (rhs), std::data (rhs) + std::size (rhs))
        { ; }


        //---------------------------------------------------------------------
        template <typename ValueT, typename AllocatorT>
        view (std::vector<ValueT,AllocatorT> &rhs):
            view (std::data (rhs), std::data (rhs) + std::size (rhs))
        { ; }


        //---------------------------------------------------------------------
        view&
        operator= (const view &rhs) noexcept
        {
            m_begin = rhs.m_begin;
            m_end   = rhs.m_end;
            return *this;
        }


        //---------------------------------------------------------------------
        view&
        operator= (view &&rhs) noexcept
        {
            m_begin = rhs.m_begin;
            m_end = rhs.m_end;

            rhs.m_begin = BeginT{};
            rhs.m_end = EndT{};

            return *this;
        };


        ///////////////////////////////////////////////////////////////////////
        constexpr BeginT begin (void) noexcept { return m_begin; }
        constexpr EndT end   (void) noexcept { return m_end;   }

        //---------------------------------------------------------------------
        constexpr const BeginT begin (void) const noexcept { return cbegin (); }
        constexpr const EndT end   (void) const noexcept { return cend   (); }

        //---------------------------------------------------------------------
        constexpr const BeginT cbegin (void) const noexcept { return m_begin; }
        constexpr const EndT cend   (void) const noexcept { return m_end;   }

        //---------------------------------------------------------------------
        auto data (void)       { return begin (); }
        auto data (void) const { return begin (); }


        ///////////////////////////////////////////////////////////////////////
        constexpr bool
        empty (void) const noexcept
        {
            return m_begin == m_end;
        }

        //---------------------------------------------------------------------
        // the return type of size _should_ be whatever std::distance returns,
        // or something else that makes sense for the iterators we've been
        // handed.
        //
        // but it's a pain in the arse to use sizes that aren't unsigned given
        // that other libraries need to compare sizes pretty often and
        // everything else in the world tends to be unsigned.
        constexpr auto
        size (void) const noexcept
        {
            return static_cast<std::size_t> (std::distance (m_begin, m_end));
        }

        //---------------------------------------------------------------------
        [[gnu::warn_unused_result]] constexpr auto
        redim (int count) const
        {
            assert (count > 0);
            if (count > size ())
                throw std::invalid_argument ("redim to higher size not allowed");
            return view { m_begin, m_begin + count };
        };


        //---------------------------------------------------------------------
        util::view<BeginT,EndT>
        operator- (util::view<BeginT,BeginT> prefix) const
        {
            assert (prefix.begin () == begin ());
            return { prefix.end (), end () };
        }


        //---------------------------------------------------------------------
        util::view<BeginT,EndT>
        operator- (BeginT newbegin) const noexcept
        {
            return { newbegin, end () };
        }


        ///////////////////////////////////////////////////////////////////////
        template <
            typename ValueT,
            typename = std::enable_if_t<
                std::is_pointer_v<BeginT> &&
                    sizeof (*std::declval<BeginT> ()) == sizeof (ValueT)
                ,
                void
            >
        >
        view<ValueT*>
        cast (void) const
        {
            return {
                reinterpret_cast<ValueT*> (m_begin),
                reinterpret_cast<ValueT*> (m_end)
            };
        }



        ///////////////////////////////////////////////////////////////////////
        constexpr auto&
        operator[] (size_t idx)& noexcept
        {
            auto it = begin ();
            std::advance (it, idx);
            return *it;
        }

        //---------------------------------------------------------------------
        constexpr auto&
        operator[] (size_t idx) const& noexcept
        {
            auto it = begin ();
            std::advance (it, idx);
            return *it;
        }

    private:
        ///////////////////////////////////////////////////////////////////////
        BeginT m_begin;
        EndT m_end;
    };


    //-------------------------------------------------------------------------
    template <typename ValueT, std::size_t N>
    view (const ValueT(&)[N]) -> view<const ValueT*,const ValueT*>;


    //-------------------------------------------------------------------------
    view (const char*) -> view<const char*>;

    view (char*) -> view<char*>;


    //-------------------------------------------------------------------------
    template <
        typename IteratorT,
        typename SizeT,
        typename = std::enable_if_t<
            std::is_integral_v<SizeT>,void
        >
    >
    view (IteratorT, SizeT) -> view<IteratorT,IteratorT>;


    template <typename CharT, typename Traits, typename Allocator>
    view (std::basic_string<CharT,Traits,Allocator> &) -> view<typename Allocator::pointer>;


    template <typename CharT, typename Traits, typename Allocator>
    view (const std::basic_string<CharT,Traits,Allocator> &) -> view<typename Allocator::const_pointer>;

    template <typename ValueT, typename AllocatorT>
    view (std::vector<ValueT,AllocatorT>&) -> view<typename AllocatorT::pointer>;


    template <typename ValueT, typename AllocatorT>
    view (const std::vector<ValueT,AllocatorT>&) -> view<typename AllocatorT::const_pointer>;


    ///////////////////////////////////////////////////////////////////////////
    template <typename ValueT, size_t N>
    auto
    make_view (const ValueT (&arr)[N])
    {
        return util::view<const ValueT*> (arr + 0, arr + N);
    }

    //-------------------------------------------------------------------------
    template <typename ContainerT>
    auto
    make_view (ContainerT &t)
    {
        return util::view<decltype(std::begin (t))> { std::begin (t), std::end (t) };
    }

    //-------------------------------------------------------------------------
    template <typename ContainerT>
    auto
    make_view (const ContainerT &t)
    {
        return util::view<decltype(std::cbegin (t))> { std::cbegin (t), std::cend (t) };
    }


    //-------------------------------------------------------------------------
    // disable the possibility of creating a view to a temporary. note that
    // this only works if an lval version has already been defined otherwise
    // universal reference rules will capture both lval and rval here.
    template <typename ContainerT>
    auto
    make_view (ContainerT&&) = delete;


    ///////////////////////////////////////////////////////////////////////////
    template <typename ContainerT>
    auto
    make_cview (const ContainerT &t)
    {
        return make_view (t);
        //return util::view<decltype(std::cbegin (t))> { std::cbegin (t), std::cend (t) };
    }


    //-------------------------------------------------------------------------
    template <typename BeginT, typename EndT>
    auto
    make_view (BeginT first, EndT last)
    {
        return view<BeginT, EndT> {first, last};
    }

    //-------------------------------------------------------------------------
    template <typename ValueT>
    auto
    make_cview (ValueT *first, ValueT *last)
    {
        return view<const ValueT*> {first, last};
    }

    ///////////////////////////////////////////////////////////////////////////
    inline
    view<const char*> make_view (const char *str)
    {
        return { str, str + strlen (str) };
    }


    //-------------------------------------------------------------------------
    inline
    view<char*> make_view (char *str)
    {
        return { str, str + strlen (str) };
    }


    //-------------------------------------------------------------------------
    template <typename CharT, typename TraitsT, typename AllocT>
    view<const CharT*>
    make_view (const std::basic_string<CharT,TraitsT,AllocT> &str)
    {
        return {
            std::data (str),
            std::data (str) + std::size (str)
        };
    }


    //-------------------------------------------------------------------------
    template <typename CharT, typename TraitsT, typename AllocT>
    view<CharT*>
    make_view (std::basic_string<CharT,TraitsT,AllocT> &str)
    {
        return {
            std::data (str),
            std::data (str) + std::size (str)
        };
    }


    //-------------------------------------------------------------------------
    template <typename CharT, typename TraitsT, typename AllocT>
    view<const CharT*>
    make_view (const std::basic_string<CharT,TraitsT,AllocT>&&) = delete;


    //-------------------------------------------------------------------------
    template <typename CharT, typename TraitsT, typename AllocT>
    view<CharT*>
    make_view (std::basic_string<CharT,TraitsT,AllocT>&&) = delete;


    ///////////////////////////////////////////////////////////////////////////
    template <typename BeginT, typename EndT>
    constexpr bool
    operator== (const view<BeginT> &a, const view<EndT> &b)
    {
        return a.size () == b.size () &&
               std::equal (std::begin (a), std::end (a), std::begin (b));
    }


    //-------------------------------------------------------------------------
    // defer equality to the view/view operator by way of make_view
    template <typename IteratorT, typename ValueT>
    constexpr bool
    operator== (const view<IteratorT> &a, const ValueT &b)
    {
        return a == make_view (b);
    }


    //-------------------------------------------------------------------------
    // reverse the arguments and forward to the above operator. we formumlate
    // equality this way to avoid implementing the operator twice for each
    // weird case.
    template <typename IteratorT, typename ValueT>
    constexpr bool
    operator== (const ValueT &a, const view<IteratorT> &b)
    {
        return b == a;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename IteratorT, typename ValueT>
    constexpr bool
    operator!= (const util::view<IteratorT> &a, const ValueT &b)
    {
        return !(a == b);
    }


    //-------------------------------------------------------------------------
    template <typename IteratorT, typename ValueT>
    constexpr bool
    operator!= (const ValueT &a, const util::view<IteratorT> &b)
    {
        return !(a == b);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename BeginT, typename EndT>
    std::ostream&
    operator<< (std::ostream &os, view<BeginT, EndT> val)
    {
        std::copy (
            std::cbegin (val),
            std::cend   (val),
            std::ostream_iterator<typename decltype(val)::value_type> (os)
        );

        return os;
    }


    ///////////////////////////////////////////////////////////////////////////
    /// a basic stringlike comparison operator that behaves as
    /// std::string::compare would.
    ///
    /// provided so that the common case of stringlike views can be used in a
    /// std::map and similar without a great deal of work.
    inline bool
    operator< (util::view<const char*> a, util::view<const char*> b)
    {
        const auto la = std::size (a);
        const auto lb = std::size (b);

        const auto res = strncmp (
            std::data (a),
            std::data (b),
            util::min (la, lb)
        );

        return res < 0 || (res == 0 && la < lb);
    }


}


#endif
