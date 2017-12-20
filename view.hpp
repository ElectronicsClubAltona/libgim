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
 * Copyright 2015-2017 Danny Robson <danny@nerdcruft.net>
 */


#ifndef CRUFT_UTIL_VIEW_HPP
#define CRUFT_UTIL_VIEW_HPP

#include "cast.hpp"
#include "debug.hpp"
#include "types/traits.hpp"

#include <cstdlib>
#include <ostream>
#include <string>
#include <cstring>
#include <stdexcept>

namespace util {
    template <typename IteratorT>
    struct view {
    public:
        //---------------------------------------------------------------------
        using value_type = typename std::iterator_traits<
            remove_restrict_t<IteratorT>
        >::value_type;


        //---------------------------------------------------------------------
        constexpr
        view (const IteratorT &first, const IteratorT &last) noexcept:
            m_begin (first),
            m_end   (last)
        { ; }


        //---------------------------------------------------------------------
        template <std::size_t N, typename ValueT>
        view (const ValueT(&value)[N]):
            view (std::begin (value), std::end (value))
        { ; }

        //---------------------------------------------------------------------
        constexpr
        view (const view &rhs) noexcept:
            view (rhs.m_begin, rhs.m_end)
        { ; }


        //---------------------------------------------------------------------
        // technically we could get away without explicitly defining a move
        // constructor here, but by nulling rhs we can more easily use this
        // class as a base for unique owning pointers without exposing the
        // begin/end data members to them directly.
        constexpr view (view &&rhs) noexcept:
            view (std::move (rhs.m_begin), std::move (rhs.m_end))
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
        template <typename ContainerT>
        constexpr explicit
        view (ContainerT &klass):
            view (std::begin (klass), std::end (klass))
        { ; }


        //---------------------------------------------------------------------
        template <typename ContainerT>
        constexpr explicit
        view (const ContainerT &klass):
            view (std::begin (klass), std::end (klass))
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

            rhs.m_begin = IteratorT{};
            rhs.m_end = IteratorT{};

            return *this;
        };


        ///////////////////////////////////////////////////////////////////////
        constexpr IteratorT begin (void) noexcept { return m_begin; }
        constexpr IteratorT end   (void) noexcept { return m_end;   }

        //---------------------------------------------------------------------
        constexpr const IteratorT begin (void) const noexcept { return cbegin (); }
        constexpr const IteratorT end   (void) const noexcept { return cend   (); }

        //---------------------------------------------------------------------
        constexpr const IteratorT cbegin (void) const noexcept { return m_begin; }
        constexpr const IteratorT cend   (void) const noexcept { return m_end;   }

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
        constexpr auto
        size (void) const noexcept
        {
            return sign_cast<std::size_t> (std::distance (m_begin, m_end));
        }

        //---------------------------------------------------------------------
        constexpr auto
        redim (int count) const
        {
            CHECK_GT (count, 0);
            if (count > size ())
                throw std::invalid_argument ("redim to higher size not allowed");
            return view { m_begin, m_begin + count };
        };

        ///////////////////////////////////////////////////////////////////////
        constexpr value_type&
        operator[] (size_t idx)& noexcept
        {
            auto it = begin ();
            std::advance (it, idx);
            return *it;
        }

        //---------------------------------------------------------------------
        constexpr const value_type&
        operator[] (size_t idx) const& noexcept
        {
            auto it = begin ();
            std::advance (it, idx);
            return *it;
        }

    private:
        ///////////////////////////////////////////////////////////////////////
        IteratorT m_begin;
        IteratorT m_end;
    };


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
        return util::view<decltype(std::cbegin (t))> { std::cbegin (t), std::cend (t) };
    }


    //-------------------------------------------------------------------------
    template <typename IteratorT>
    auto
    make_view (IteratorT first, IteratorT last)
    {
        return view<IteratorT> {first, last};
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
    template <typename IteratorA, typename IteratorB>
    constexpr bool
    operator== (const view<IteratorA> &a, const view<IteratorB> &b)
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


    ///////////////////////////////////////////////////////////////////////////
    template <typename IteratorT>
    std::ostream&
    operator<< (std::ostream&, view<IteratorT>);
}


#endif
