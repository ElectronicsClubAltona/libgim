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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_TUPLE_HPP
#define __UTIL_TUPLE_HPP

#include "types.hpp"

#include <tuple>
#include <type_traits>


namespace util {
    ///////////////////////////////////////////////////////////////////////////
    /// call a provided object with type_tag<T> for each type in a tuple
    template <
        typename T,
        typename F,
        size_t S = 0
    >
    typename std::enable_if<
        S == std::tuple_size<T>::value,
        void
    >::type
    for_type (F)
    { }


    //-------------------------------------------------------------------------
    template<
        typename T,
        typename F,
        size_t S = 0
    >
    typename std::enable_if<
        S < std::tuple_size<T>::value,
        void
    >::type
    for_type (F f)
    {
        using E = typename std::tuple_element<S,T>::type;

        f (type_tag<E> {});
        for_type<T,F,S+1> (f);
    }


    //-------------------------------------------------------------------------
    template <typename T, typename F>
    auto for_type (F f, T t)
    { return for_type<decltype(t)> (f); }


    ///////////////////////////////////////////////////////////////////////////
    /// call a provided object with each value in a tuple
    template <
        size_t S = 0,
        typename F,
        typename ...T
    >
    typename std::enable_if<
        S == sizeof...(T),
        void
    >::type
    for_each (F, const std::tuple<T...>&)
    { }


    //-------------------------------------------------------------------------
    template<
        size_t S = 0,
        typename F,
        typename ...T
    >
    typename std::enable_if<
        S < sizeof...(T),
        void
    >::type
    for_each (F f, const std::tuple<T...> &t)
    {
        f (std::get<S> (t));
        for_each<S+1,F,T...> (f, t);
    }
}


#endif
