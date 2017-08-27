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

#include "./ops.hpp"
#include "../maths.hpp"

#include <algorithm>
#include <cstdlib>
#include <type_traits>

namespace util::coord {
    /////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T,
        template <size_t, typename> class KLASS,
        typename ...tags
    >
    struct base : public init<S,T,tags...> {
        static_assert (S > 0);
        static_assert (std::is_arithmetic<T>::value);

        using value_type = T;
        static constexpr size_t dimension = S;
        static constexpr size_t elements = S;

        /// returns the number of elements we contain
        static constexpr size_t size (void) { return S; }

        // inherit the fancy elementwise constructors from `init'.
        using init<S,T,tags...>::init;

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

        constexpr base (const base<S,T,KLASS,tags...> &rhs) = default;
        base& operator= (const base<S,T,KLASS,tags...> &rhs) = default;

        // element accessors
        T& operator[] (size_t i) { return this->data[i]; }
        constexpr const T& operator[] (size_t i) const { return this->data[i]; }

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

        //---------------------------------
        template <typename U>
        KLASS<S,U>
        cast (void) const
        {
            KLASS<S,U> out;
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
        template <std::size_t D>
        std::enable_if_t<
            D <= S, KLASS<D,T>
        >
        redim (void) const
        {
            KLASS<D,T> out;
            std::copy_n (cbegin (), D, std::begin  (out.data));
            return out;
        }


        //---------------------------------------------------------------------
        /// returns an instance with the same data, but more elements, where
        /// the new elements are initialised with values with the same index
        /// in the coordinate `fill'.
        ///
        /// explicitly requires a fill parameter so that we avoid undefined
        /// values.
        template <std::size_t D>
        std::enable_if_t<
            (D > S), KLASS<D,T>
        >
        redim (const KLASS<D,T> fill) const
        {
            KLASS<D,T> out;

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
        template <std::size_t D>
        std::enable_if_t<
            (D > S), KLASS<D,T>
        >
        redim (T fill) const
        {
            KLASS<D,T> out;

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
        KLASS<sizeof...(Indices),T>
        indices (void) const
        {
            static_assert (all (make_vector ((Indices < S)...)));
            return KLASS<sizeof...(Indices),T> { this->data[Indices]... };
        }
    };
}


#endif
