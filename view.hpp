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

namespace util {
    template <typename T>
    struct view {
    public:
        using value_type = typename std::iterator_traits<remove_restrict_t<T>>::value_type;

        constexpr
        view (T first, T last) noexcept;

        template <typename K> constexpr explicit view (      K &klass);
        template <typename K> constexpr explicit view (const K &klass);

        constexpr T& begin (void) noexcept;
        constexpr T& end   (void) noexcept;

        constexpr const T& begin (void) const noexcept;
        constexpr const T& end   (void) const noexcept;

        constexpr const T& cbegin (void) const noexcept;
        constexpr const T& cend   (void) const noexcept;

        constexpr T find (value_type) const noexcept;

        constexpr bool empty (void) const noexcept;
        constexpr size_t size (void) const noexcept;

        constexpr value_type& operator[] (size_t) noexcept;
        constexpr value_type& operator[] (size_t) const noexcept;

        bool operator== (view) const noexcept;

    private:
        T m_begin;
        T m_end;
    };

    template <typename T, size_t N>
    auto
    make_view (const T (&)[N]);

    template <typename T>
    auto
    make_view (T&);

    template <typename T>
    auto
    make_view (const T&);

    template <typename T>
    auto
    make_view (T&&) = delete;

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

#include "./view.ipp"

#endif
