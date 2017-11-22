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

#include "fwd.hpp"

#include "init.hpp"
#include "traits.hpp"
#include "../maths.hpp"

#include <algorithm>
#include <cstdlib>
#include <type_traits>


namespace util::coord {
    /////////////////////////////////////////////////////////////////////////
    // the base class for all coordinate-like types.
    //
    // SelfT should not be exposed as a template template directly because
    // some types (eg, XYZ colours) do not conform to the same template
    // parameters are others (eg, vector2f). ie, it does not make sense to
    // allow redim, or type changing on some types so they just aren't exposed.
    template <
        size_t S,
        typename T,
        typename SelfT
    >
    struct base : public init<S,T,SelfT> {
        static_assert (S > 0);
        static_assert (std::is_arithmetic<T>::value);
        static_assert (sizeof (init<S,T,SelfT>) == S * sizeof (T));

        using value_type = T;
        static constexpr size_t dimension = S;
        static constexpr size_t elements = S;

        static constexpr auto size (void) { return S; }

        // constructors
        using init<S,T,SelfT>::init;
        base () = default;

        constexpr explicit base (T val)
        { std::fill (begin (), end (), val); }

        constexpr base (const base<S,T,SelfT> &rhs) = default;
        base& operator= (const base<S,T,SelfT> &rhs) = default;

        // element accessors
        T& operator[] (size_t i) { return this->data[i]; }
        constexpr const T& operator[] (size_t i) const { return this->data[i]; }
        T& operator[] (int i) { return this->data[i]; }
        constexpr const T& operator[] (int i) const { return this->data[i]; }

        auto cbegin (void) const { return std::cbegin (this->data); }
        auto cend   (void) const { return std::cend   (this->data); }

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


        //---------------------------------------------------------------------
        template <
            typename K,
            typename = std::enable_if_t<is_coord_v<K>,void>
        >
        K as (void) const
        {
            static_assert (K::elements == elements);
            K k;
            std::copy (begin (), end (), k.begin ());
            return k;
        }


        //---------------------------------
        template <typename U>
        auto
        cast (void) const
        {
            typename revalue_type<SelfT>::template type<U> out;

            std::copy (std::cbegin (this->data),
                       std::cend   (this->data),
                       std::begin  (out.data));
            return out;
        }

        ///////////////////////////////////////////////////////////////////////
        // redimension
        template <
            size_t D,
            typename _sfinae = SelfT
        >
        std::enable_if_t<
            has_redim_v<_sfinae>,
            redim_t<_sfinae,D>
        >
        redim (void) const
        {
            redim_t<SelfT,D> out;

            std::copy_n (std::cbegin (this->data),
                         min (S, D),
                         std::begin  (out.data));
            return out;
        }


        //---------------------------------------------------------------------
        template<size_t D,typename _sfinae = SelfT>
        std::enable_if_t<
            has_redim_v<_sfinae>,
            redim_t<_sfinae,D>
        >
        redim (const redim_t<_sfinae,D> fill) const
        {
            redim_t<SelfT,D> out;

            static constexpr auto L1 = min (S, D);
            static constexpr auto L2 = D - L1;

            std::copy_n (std::cbegin (this->data),
                         L1,
                         std::begin (out.data));

            std::copy_n (fill.data + L1,
                         L2,
                         out.data + L1);
            return out;
        }

        //---------------------------------------------------------------------
        template <
            size_t D,
            typename _sfinae = SelfT
        >
        std::enable_if_t<
            has_redim_v<_sfinae>,
            redim_t<_sfinae,D>
        >
        redim (T fill) const
        {
            redim_t<SelfT,D> out;

            auto cursor = std::copy_n (std::cbegin (this->data),
                                       min (S, D),
                                       std::begin (out.data));
            std::fill (cursor, std::end (out.data), fill);

            return out;
        }
    };
}

#include "ops.hpp"

#endif
