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
#include "../maths.hpp"

#include <algorithm>
#include <cstdlib>

namespace util { namespace coord {
    /////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T, template <size_t, typename> class KLASS, typename ...tags>
    struct base : public init <S,T,tags...> {
        static_assert (S > 0, "coord dimensions must be strictly positive");

        typedef T value_type;
        static constexpr size_t dimension = S;
        static constexpr size_t elements = S;

        size_t size (void) const { return S; }

        // constructors
        using init<S,T,tags...>::init;
        base () = default;

        explicit base (T val)
        { std::fill (std::begin (this->data), std::end (this->data), val); }

        base (const base<S,T,KLASS,tags...> &rhs) = default;
        base& operator= (const base<S,T,KLASS,tags...> &rhs) = default;

        // element accessors
        T& operator[] (size_t i)       { return this->data[i]; }
        T  operator[] (size_t i) const { return this->data[i]; }

        const T* begin (void) const { return std::begin (this->data); }
        const T* end   (void) const { return std::end   (this->data); }

        T* begin (void) { return std::begin (this->data); }
        T* end   (void) { return std::end   (this->data); }

        ///////////////////////////////////////////////////////////////////////
        // conversions
        template <template <size_t, typename> class K>
        K<S,T> as (void) const
        {
            K<S,T> k;
            std::copy (begin (), end (), k.begin ());
            return k;
        }

        //---------------------------------------------------------------------
        template <typename U>
        KLASS<S,U>
        cast (void) const
        {
            KLASS<S,U> out;
            std::copy (std::begin (this->data),
                       std::end   (this->data),
                       std::begin (out.data));
            return out;
        }

        ///////////////////////////////////////////////////////////////////////
        // redimension
        template <size_t D>
        KLASS<D,T>
        redim (void) const
        {
            KLASS<D,T> out;
            std::copy_n (std::begin (this->data),
                         min (S, D),
                         std::begin (out.data));
            return out;
        }


        //---------------------------------------------------------------------
        template<size_t D>
        KLASS<D,T>
        redim (const KLASS<D,T> fill) const
        {
            KLASS<D,T> out;

            static constexpr auto L1 = min (S, D);
            static constexpr auto L2 = D - L1;

            std::copy_n (std::begin (this->data),
                         L1,
                         std::begin (out.data));

            std::copy_n (fill.data + L1,
                         L2,
                         out.data + L1);
            return out;
        }

        //---------------------------------------------------------------------
        template <size_t D>
        KLASS<D,T>
        redim (T fill) const
        {
            KLASS<D,T> out;

            auto cursor = std::copy_n (std::begin (this->data),
                                       min (S, D),
                                       std::begin (out.data));
            std::fill (cursor, std::end (out.data), fill);

            return out;
        }
    };
} }


#endif
