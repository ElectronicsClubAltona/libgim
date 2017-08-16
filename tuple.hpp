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
#include "variadic.hpp"

#include <tuple>
#include <type_traits>


namespace util::tuple {
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
    // unpack a tuple as function arguments
    namespace detail {
        template <
            typename Func,
            typename ...Args,
            size_t ...I
        >
        auto
        call (const Func &func, std::tuple<Args...> args, std::index_sequence<I...>)
        {
            // quiet unused variable warning with zero args
            (void)args;

            // XXX: Use `std::invoke' when gcc catches up with c++17
            return func (std::get<I> (args)...);
        }
    }

    template <
        typename Func,
        typename ...Args
    >
    auto
    call (const Func &func, std::tuple<Args...> args)
    {
        return detail::call (func, args, std::make_index_sequence<sizeof...(Args)> {});
    }


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

    ///////////////////////////////////////////////////////////////////////////////
    /// Statically map the member types of a tuple via F<>::type
    ///
    /// T: tuple type
    /// F: type mapping object, conversion uses F<>::type
    /// I: tuple indexing helper
    template <
        typename T,
        template <
            typename
        > class F,
        typename I = std::make_index_sequence<
            std::tuple_size<T>::value
        >
    >
    struct map;


    //-----------------------------------------------------------------------------
    template <
        typename T,
        template <
            typename
        > class F,
        size_t ...I
    >
    struct map<
        T,
        F,
        std::index_sequence<I...>
    > {
        typedef std::tuple<
            typename F<
                typename std::tuple_element<I, T>::type
            >::type...
        > type;
    };


    ///////////////////////////////////////////////////////////////////////////
    /// do nothing with a set of parameters.
    ///
    /// useful for temporarily silencing unused argument warnings in parameter
    /// packs, or for avoiding assignment of [[gnu::warn_unused_result]] to a
    /// temporary value we'd just cast to void anyway (GCC#66425).
    ///
    /// it is guaranteed that this function will never be defined out in
    /// debug/release/whatever builds. so it is safe to use to guarantee
    /// parameter evaluation.
    inline void
    ignore (void)
    { ; }

    //-------------------------------------------------------------------------
    template <typename T, typename ...Args>
    void
    ignore (T, Args ...args)
    {
        ignore (std::forward<Args> (args)...);
    };


    ///////////////////////////////////////////////////////////////////////////
    /// query the index of the first occurrence of type `T' in the tuple type
    /// `TupleT'.
    ///
    /// if the query type does not occur in the tuple type a compiler error
    /// should be generated.
    template <class T, class TupleT>
    struct index;


    //-------------------------------------------------------------------------
    template <class T, class ...Types>
    struct index<T, std::tuple<T, Types...>> {
        static constexpr std::size_t value = 0;
    };


    //-------------------------------------------------------------------------
    template <class T, class U, class ...Types>
    struct index<T,std::tuple<U, Types...>> {
        static constexpr std::size_t value = 1 + index<T, std::tuple<Types...>>::value;
    };
};


#endif
