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
 * Copyright 2012-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COORD_BASE_HPP
#define __UTIL_COORD_BASE_HPP

#include "init.hpp"
#include "../maths.hpp"

#include <algorithm>
#include <cstdlib>

namespace util { namespace coord {
    /////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T,
        template <size_t, typename> class KLASS,
        typename ...tags
    >
    struct base : public init<S,T,tags...> {
        static_assert (S > 0, "coord dimensions must be strictly positive");

        using value_type = T;
        static constexpr size_t dimension = S;
        static constexpr size_t elements = S;

        static constexpr size_t size (void) { return S; }

        // constructors
        using init<S,T,tags...>::init;
        base () = default;

        constexpr explicit base (T val)
        { std::fill (begin (), end (), val); }

        constexpr base (const base<S,T,KLASS,tags...> &rhs) = default;
        base& operator= (const base<S,T,KLASS,tags...> &rhs) = default;

        // element accessors
        T& operator[] (size_t i) { return this->data[i]; }
        constexpr const T& operator[] (size_t i) const { return this->data[i]; }

        auto begin (void) const { return std::begin (this->data); }
        auto end   (void) const { return std::end   (this->data); }

        auto begin (void) { return std::begin (this->data); }
        auto end   (void) { return std::end   (this->data); }

        const T& front (void) const { return this->data[0]; }
        T& front (void) { return this->data[0]; }

        const T& back (void) const { return this->data[S-1]; }
        T& back (void) { return this->data[S-1]; }

        ///////////////////////////////////////////////////////////////////////
        // conversions
        template <template <size_t, typename> class K>
        K<S,T> as (void) const
        {
            K<S,T> k;
            std::copy (begin (), end (), k.begin ());
            return k;
        }

        //---------------------------------
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
