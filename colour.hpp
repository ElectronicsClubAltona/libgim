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
 * Copyright 2010-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COLOUR_HPP
#define __UTIL_COLOUR_HPP

#include "coord.hpp"
#include "introspection.hpp"

#include <ostream>
#include <type_traits>


namespace util {
    /// An abstract colour POD type componsed of S components of type T.
    ///
    /// Not to be used directly, instead the use of derived types is required.
    /// This exists purely to simplify generic colour code.
    template <
        size_t S,
        typename T,
        typename SelfT
    >
    struct colour : coord::base<S,T,SelfT> {
        using coord::base<S,T,SelfT>::base;

        template <typename U>
        auto
        cast (void) const
        {
            ::util::revalue_t<SelfT,U> ret;
            std::transform (std::begin (*this),
                            std::end   (*this),
                            std::begin (ret),
                            renormalise<T,U>);
            return ret;
        }
    };

    template <typename T>
    struct util::coord::store<1,T,srgba<1,T>> {
        union { struct { T r; }; T data[1]; };
    };
    template <typename T>
    struct util::coord::store<2,T,srgba<2,T>> {
        union { struct { T r, g; }; T data[2]; };
    };
    template <typename T>
    struct util::coord::store<3,T,srgba<3,T>> {
        union { struct { T r, g, b; }; T data[3]; };
    };

    template <typename T>
    struct util::coord::store<4,T,srgba<4,T>> {
        union { struct { T r, g, b, a; }; T data[4]; };
    };

    template <size_t S, typename T> struct srgba : colour<S,T,srgba<S,T>> {
        using colour<S,T,srgba<S,T>>::colour;
    };

    using srgba3f = srgba<3,float>;
    using srgba4f = srgba<4,float>;

    using srgba3u = srgba<3,uint8_t>;
    using srgba4u = srgba<4,uint8_t>;

    template <size_t S, typename T> struct hsva  : colour<S,T,hsva<S,T>> {};

    template <size_t S, typename T>
    struct redim_type<
        srgba<S,T>
    > { template <size_t _S> using type = srgba<_S,T>; };

    template <size_t S, typename T>
    struct revalue_type<srgba<S,T>> {
        template <typename _T>
        using type = srgba<S,_T>;
    };


    template <typename> struct is_colour : public std::false_type {};
    template <
        size_t S,
        typename T,
        template <
            size_t,
            typename
        > typename ColourT
    > struct is_colour<ColourT<S,T>>
    :std::conditional_t<
        std::is_base_of_v<
            colour<S,T,ColourT<S,T>>,
            ColourT<S,T>
        >,
        std::true_type,
        std::false_type
    > {};


    template <typename T>
    constexpr auto is_colour_v = is_colour<T>::value;
}

#include "colour.ipp"

#endif
