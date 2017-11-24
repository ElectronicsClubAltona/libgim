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
 * Copyright 2012-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_COORD_BASE_HPP
#define CRUFT_UTIL_COORD_BASE_HPP

#include "fwd.hpp"

#include "ops.hpp"
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
        std::size_t S,
        typename T,
        typename SelfT
    >
    struct base : public init<S,T,SelfT> {
        static_assert (S > 0);
        static_assert (std::is_arithmetic<T>::value);
        static_assert (sizeof (init<S,T,SelfT>) == S * sizeof (T));

        using value_type = T;
        static constexpr std::size_t dimension = S;
        static constexpr std::size_t elements = S;

        /// returns the number of elements we contain
        static constexpr auto size (void) { return S; }

        // constructors
        using init<S,T,SelfT>::init;

        /// constructs, but does not initialise, the data.
        ///
        /// used to avoid unnecessary initialisation in many situations where
        /// we have arrays of these types that are about to be overwritten. it
        /// is a very important performance optimisation.
        base () = default;

        /// constructs an instance where all elements are initialised to `val'.
        constexpr explicit
        base (T fill)
        {
            for (decltype(S) i = 0; i < S; ++i)
                this->data[i] = fill;
        }

        constexpr base (const base<S,T,SelfT> &rhs) = default;
        base& operator= (const base<S,T,SelfT> &rhs)& = default;
        base& operator= (const T t)&
        {
            for (auto v: *this)
                v = t;
            return *this;
        }

        // element accessors
        constexpr T& operator[] (size_t i)& noexcept { return this->data[i]; }
        constexpr T& operator[] (int i)& noexcept { return this->data[i]; }

        constexpr const T& operator[] (size_t i) const& noexcept { return this->data[i]; }
        constexpr const T& operator[] (int i) const& noexcept { return this->data[i]; }

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
        template <template <std::size_t, typename> class K>
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
        /// returns an instance with the same data, but truncated to `D'
        /// elements
        ///
        /// explicitly does not allow a fill parameter given it can't be used
        /// when reducing dimensions.
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
        /// returns an instance with the same data, but more elements, where
        /// the new elements are initialised with values with the same index
        /// in the coordinate `fill'.
        ///
        /// explicitly requires a fill parameter so that we avoid undefined
        /// values.
        template<size_t D,typename _sfinae = SelfT>
        std::enable_if_t<
            has_redim_v<_sfinae>,
            redim_t<_sfinae,D>
        >
        redim (const redim_t<_sfinae,D> fill) const
        {
            redim_t<SelfT,D> out;

            auto next = std::copy (cbegin (), cend (), std::begin (out));
            std::copy (std::cbegin (fill) + S, std::cend (fill), next);
            return out;
        }

        //---------------------------------------------------------------------
        /// returns an instance with the same data, but more elements, where
        /// all the new elemenst are initialised with the scalar `fill'.
        ///
        /// explicitly requires a fill parameter so that we avoid undefined
        /// values.
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

            auto next = std::copy (cbegin (), cend (), std::begin (out));
            std::fill (next, std::end (out), fill);
            return out;
        }


        ///////////////////////////////////////////////////////////////////////
        /// returns an instance with elements specified by the Indices
        /// parameter. eg, point2f p{}.indices<0,2> would return {p.x, p.z}.
        ///
        /// it's ugly as sin, but simplifies some situations where we don't
        /// want a temporary.
        template <std::size_t ...Indices>
        constexpr auto
        indices (void) const
        {
            static_assert (
                all (make_vector ((Indices < S)...)),
                "indices must fall within the defined range for the type"
            );

            return redim_t<SelfT,sizeof...(Indices)> {
                this->data[Indices]...
            };
        }
    };
}

#endif
