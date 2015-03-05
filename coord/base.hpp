/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COORD_BASE_HPP
#define __UTIL_COORD_BASE_HPP

#include "init.hpp"

#include <algorithm>
#include <cstdlib>

namespace util { namespace coord {
    /////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T, typename ...tags>
    struct base : public init <S,T,tags...> {
        static_assert (S > 0, "coord dimensions must be strictly positive");

        typedef T value_type;
        static constexpr size_t dimension = S;
        static constexpr size_t elements = S;

        size_t size (void) const { return S; }

        // constructors
        using init<S,T,tags...>::init;
        base () = default;

        explicit base (T v)
        { std::fill (std::begin (this->data), std::end (this->data), v); }

        base (const base<S,T,tags...> &rhs) = default;
        base& operator= (const base<S,T,tags...> &rhs) = default;

        // element accessors
        T& operator[] (size_t i)       { return this->data[i]; }
        T  operator[] (size_t i) const { return this->data[i]; }

        const T* begin (void) const { return std::begin (this->data); }
        const T* end   (void) const { return std::end   (this->data); }

        T* begin (void) { return std::begin (this->data); }
        T* end   (void) { return std::end   (this->data); }
    };
} }


#endif
