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
 * Copyright 2015-2016 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_VIEW_HPP
#define __UTIL_VIEW_HPP

#include "./types/traits.hpp"

#include <cstdlib>
#include <ostream>
#include <string>
#include <cstring>

namespace util {
    template <typename T>
    struct view {
    public:
        using value_type = typename std::iterator_traits<remove_restrict_t<T>>::value_type;

        constexpr
        view (T first, T last) noexcept:
            m_begin (first),
            m_end   (last)
        { ; }

        template <typename K>
        constexpr explicit
        view (K &klass):
            m_begin (std::begin (klass)),
            m_end   (std::end   (klass))
        { ; }

        template <typename K>
        constexpr explicit
        view (const K &klass):
            m_begin (std::begin (klass)),
            m_end   (std::end   (klass))
        { ; }

        constexpr T& begin (void) noexcept { return m_begin; }
        constexpr T& end   (void) noexcept { return m_end;   }

        constexpr const T& begin (void) const noexcept { return cbegin (); }
        constexpr const T& end   (void) const noexcept { return cend   (); }

        constexpr const T& cbegin (void) const noexcept { return m_begin; }
        constexpr const T& cend   (void) const noexcept { return m_end;   }

        auto data (void)       { return begin (); }
        auto data (void) const { return begin (); }

        constexpr T
        find (const value_type &v) const noexcept
        {
            for (T i = cbegin (); i != cend (); ++i)
                if (*i == v)
                    return i;
            return cend ();
        }

        constexpr bool
        empty (void) const noexcept
        {
            return m_begin == m_end;
        }

        constexpr auto
        size (void) const noexcept
        {
            return std::distance (m_begin, m_end);
        }

        constexpr value_type&
        operator[] (size_t idx) noexcept
        {
            auto it = begin ();
            std::advance (it, idx);
            return *it;
        }

        constexpr const value_type&
        operator[] (size_t idx) const noexcept
        {
            auto it = begin ();
            std::advance (it, idx);
            return *it;
        }

        bool
        operator== (const view &rhs) const noexcept
        {
            return rhs.m_begin == m_begin &&
                   rhs.m_end   == m_end;
        }

    private:
        T m_begin;
        T m_end;
    };

    template <typename T, size_t N>
    auto
    make_view (const T (&arr)[N])
    {
        return util::view<const T*> (arr + 0, arr + N);
    }

    template <typename T>
    auto
    make_view (T &t)
    {
        return util::view<decltype(std::begin (t))> { std::begin (t), std::end (t) };
    }

    template <typename T>
    auto
    make_view (const T &t)
    {
        return util::view<decltype(std::cbegin (t))> { std::cbegin (t), std::cend (t) };
    }

    template <typename T>
    auto
    make_view (T&&) = delete;

    template <typename T>
    auto
    make_cview (const T &t)
    {
        return util::view<decltype(std::cbegin (t))> { std::cbegin (t), std::cend (t) };
    }

    template <typename T>
    auto
    make_view (T first, T last)
    {
        return view<T> {first, last};
    }

    template <typename T>
    auto
    make_cview (T *first, T *last)
    {
        return view<const T*> {first, last};
    }

    // string conversions
    inline
    view<const char*> make_view (const char *str)
    {
        return { str, str + strlen (str) };
    }

    inline
    view<char*> make_view (char *str)
    {
        return { str, str + strlen (str) };
    }

    template <typename CharT, typename TraitsT, typename AllocT>
    view<const CharT*>
    make_view (const std::basic_string<CharT,TraitsT,AllocT> &str)
    {
        return {
            std::data (str),
            std::data (str) + std::size (str)
        };
    }

    template <typename CharT, typename TraitsT, typename AllocT>
    view<CharT*>
    make_view (std::basic_string<CharT,TraitsT,AllocT> &str)
    {
        return {
            std::data (str),
            std::data (str) + std::size (str)
        };
    }

    template <typename CharT, typename TraitsT, typename AllocT>
    view<const CharT*>
    make_view (const std::basic_string<CharT,TraitsT,AllocT>&&) = delete;

    template <typename CharT, typename TraitsT, typename AllocT>
    view<CharT*>
    make_view (std::basic_string<CharT,TraitsT,AllocT>&&) = delete;

    // boolean operationrs
    template <typename T>
    bool operator!= (view<T>, view<T>);

    bool operator== (const std::string&, view<const char*>);
    bool operator== (const std::string&, view<char*>);
    bool operator== (const std::string&, view<std::string::const_iterator>);
    bool operator== (const std::string&, view<std::string::iterator>);

    bool operator== (view<const char*>,  const std::string&);
    bool operator== (view<char*>,        const std::string&);
    bool operator== (view<std::string::const_iterator>, const std::string&);
    bool operator== (view<std::string::iterator>, const std::string&);

    template <typename T>
    std::ostream&
    operator<< (std::ostream&, view<T>);
}

#endif
