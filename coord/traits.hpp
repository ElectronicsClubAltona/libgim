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

#ifndef CRUFT_UTIL_COORD_TRAITS_HPP
#define CRUFT_UTIL_COORD_TRAITS_HPP

#include "fwd.hpp"

#include <cstddef>
#include <type_traits>

namespace util {
    ///////////////////////////////////////////////////////////////////////
    // operation traits
    template <typename, typename>
    struct result { };

    //-------------------------------------------------------------------------
    #define RESULT_T(ArgA,ArgB,ResultT) \
    template <          \
        std::size_t S,  \
    typename ValueA,\
    typename ValueB\
    > struct result<\
        ArgA<S,ValueA>,\
        ArgB<S,ValueB>\
    >  {\
    using type = ResultT<S,std::common_type_t<ValueA,ValueB>>; \
};

#if 0
    template <
        std::size_t S,
        typename ValueA,
        typename ValueB
    > struct result<
        srgba<S,ValueA>,
        srgba<S,ValueB>
    >  {
        using type = srgba<S,std::common_type_t<ValueA,ValueB>>;
    };


    template <std::size_t S, typename T> struct result<srgba<S,T>, vector<S,T>> { using type = srgba<S,T>;  };
    template <std::size_t S, typename T> struct result<vector<S,T>,srgba<S,T>>  { using type = srgba<S,T>;  };
    template <std::size_t S, typename T> struct result<extent<S,T>,extent<S,T>> { using type = extent<S,T>; };
    template <std::size_t S, typename T> struct result<extent<S,T>,vector<S,T>> { using type = extent<S,T>; };
    template <std::size_t S, typename T> struct result<point<S,T>, extent<S,T>> { using type = point <S,T>; };
    template <std::size_t S, typename T> struct result<point<S,T>, vector<S,T>> { using type = point <S,T>; };
    template <std::size_t S, typename T> struct result<vector<S,T>,point<S,T>>  { using type = point <S,T>; };
    template <std::size_t S, typename T> struct result<vector<S,T>,vector<S,T>> { using type = vector<S,T>; };
#endif

    RESULT_T(extent,extent,extent)
    RESULT_T(extent,vector,extent)
    RESULT_T(point,extent,point)
    RESULT_T(point,vector,point)
    RESULT_T(srgba,vector,srgba)
    RESULT_T(srgba,srgba,srgba)
    RESULT_T(vector,point,point)
    RESULT_T(vector,srgba,srgba)
    RESULT_T(vector,vector,vector)


    template <
        typename A, typename B
    >
    using result_t = typename result<A,B>::type;


    ///////////////////////////////////////////////////////////////////////////
    template <typename A, typename B, typename = std::void_t<>>
    struct has_result : public std::false_type {};


    template <typename CoordA, typename CoordB>
    struct has_result<
        CoordA,
        CoordB,
        std::void_t<
            result_t<CoordA,CoordB>
        >
    > : public std::true_type {};


    template <typename A, typename B>
    constexpr auto has_result_v = has_result<A,B>::value;


    //---------------------------------------------------------------------
    template <typename>
    struct has_norm : public std::false_type { };

    template <std::size_t S, typename T> struct has_norm<vector<S,T>> : public std::true_type { };

    template <typename T>
    constexpr auto has_norm_v = has_norm<T>::value;


    //---------------------------------------------------------------------
    template <typename T>
    struct has_scalar_op : public std::false_type { };

    template <std::size_t S, typename T> struct has_scalar_op<srgba<S,T>>  : public std::true_type { };
    template <std::size_t S, typename T> struct has_scalar_op<extent<S,T>> : public std::true_type { };
    template <std::size_t S, typename T> struct has_scalar_op<point<S,T>>  : public std::true_type { };
    template <std::size_t S, typename T> struct has_scalar_op<vector<S,T>> : public std::true_type { };

    template <typename T>
    constexpr auto has_scalar_op_v = has_scalar_op<T>::value;


    template <class> struct is_coord : std::false_type { };

    template <typename T> struct is_coord<const T> : is_coord<T> {};
    template <typename T> struct is_coord<T&> : is_coord<T> {};

    template <std::size_t S, typename T> struct is_coord<point<S,T>>      : std::true_type { };
    template <std::size_t S, typename T> struct is_coord<extent<S,T>>     : std::true_type { };
    template <std::size_t S, typename T> struct is_coord<vector<S,T>>     : std::true_type { };
    template <std::size_t S, typename T> struct is_coord<srgba<S,T>> : std::true_type { };

    template <class K>
    constexpr bool
    is_coord_v = is_coord<K>::value;


    ///////////////////////////////////////////////////////////////////////////
    template <typename T> struct has_redim : std::false_type {};


    //---------------------------------------------------------------------
    template <std::size_t S, typename T> struct has_redim< point<S,T>> : public std::true_type {};
    template <std::size_t S, typename T> struct has_redim<vector<S,T>> : public std::true_type {};
    template <std::size_t S, typename T> struct has_redim<extent<S,T>> : public std::true_type {};

    //---------------------------------------------------------------------
    template <typename T> constexpr auto has_redim_v = has_redim<T>::value;


    ///////////////////////////////////////////////////////////////////////////
    template <typename> struct redim_type {};


    //---------------------------------------------------------------------
    template <std::size_t S, typename T> struct redim_type<point<S,T>>
    { template <std::size_t _S> using type = point<_S,T>; };

    template <std::size_t S, typename T> struct redim_type<vector<S,T>>
    { template <std::size_t _S> using type = vector<_S,T>; };

    template <std::size_t S, typename T> struct redim_type<extent<S,T>>
    { template <std::size_t _S> using type = extent<_S,T>; };


    //---------------------------------------------------------------------
    template <
        typename Self,
        std::size_t S
    >
    using redim_t = typename redim_type<Self>::template type<S>;


    ///////////////////////////////////////////////////////////////////////////
    template <typename> struct revalue_type {};


    //-------------------------------------------------------------------------
    template <std::size_t S, typename T>
    struct revalue_type<point<S,T>> {
        template <typename _T> using type = point<S,_T>;
    };

    template <std::size_t S, typename T>
    struct revalue_type<vector<S,T>> {
        template <typename _T> using type = vector<S,_T>;
    };

    template <std::size_t S, typename T>
    struct revalue_type<extent<S,T>> {
        template <typename _T> using type = extent<S,_T>;
    };


    //-------------------------------------------------------------------------
    template <typename Self, typename T>
    using revalue_t = typename revalue_type<Self>::template type<T>;


    ///////////////////////////////////////////////////////////////////////////
    template <typename,typename=void> struct arity {};


    template <typename T>
    struct arity<T,std::enable_if_t<std::is_arithmetic_v<T>,void>
    > :std::integral_constant<std::size_t, 1>
    { };


    template <typename T>
    struct arity<
        T,std::enable_if_t<is_coord_v<T>,void>
    > :std::integral_constant<std::size_t,T::elements>
    { };


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    constexpr auto arity_v = arity<T>::value;
}

#endif
