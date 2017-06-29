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

#ifndef __UTIL_COORDS_OPS
#define __UTIL_COORDS_OPS

#include "./fwd.hpp"

#include "../debug.hpp"
#include "../maths.hpp"
#include "../preprocessor.hpp"
#include "../types/bits.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iterator>

namespace util {
    ///////////////////////////////////////////////////////////////////////
    // operation traits
    namespace coord {
        template <
            template <size_t,typename> class A,
            template <size_t,typename> class B
        >
        struct result { };

        //-------------------------------------------------------------------------
        template <> struct result<colour,colour> { template <size_t S, typename T> using type = colour<S,T>; };
        template <> struct result<extent,extent> { template <size_t S, typename T> using type = extent<S,T>; };
        template <> struct result<extent,vector> { template <size_t S, typename T> using type = extent<S,T>; };
        template <> struct result<point,extent>  { template <size_t S, typename T> using type = point <S,T>; };
        template <> struct result<point,vector>  { template <size_t S, typename T> using type = point <S,T>; };
        template <> struct result<vector,point>  { template <size_t S, typename T> using type = point <S,T>; };
        template <> struct result<vector,vector> { template <size_t S, typename T> using type = vector<S,T>; };

        template <
            template <size_t,typename> class A,
            template <size_t,typename> class B
        >
        using result_t = typename result<A,B>::type;


        //---------------------------------------------------------------------
        template <template <size_t,typename> class K>
        struct has_norm : public std::false_type { };

        template <> struct has_norm<vector>     : public std::true_type { };

        template <template <size_t,typename> class K>
        constexpr auto has_norm_v = has_norm<K>::value;


        //---------------------------------------------------------------------
        template <template <size_t,typename> class K>
        struct has_scalar_op : public std::false_type { };

        template <> struct has_scalar_op<colour>     : public std::true_type { };
        template <> struct has_scalar_op<extent>     : public std::true_type { };
        template <> struct has_scalar_op<point>      : public std::true_type { };
        template <> struct has_scalar_op<vector>     : public std::true_type { };

        template <template <size_t,typename> class K>
        constexpr auto has_scalar_op_v = has_scalar_op<K>::value;
    }

    template <class> struct is_coord : std::false_type { };

    template <size_t S, typename T> struct is_coord<point<S,T>>      : std::true_type { };
    template <size_t S, typename T> struct is_coord<extent<S,T>>     : std::true_type { };
    template <size_t S, typename T> struct is_coord<vector<S,T>>     : std::true_type { };
    template <size_t S, typename T> struct is_coord<colour<S,T>>     : std::true_type { };

    template <class K>
    constexpr bool
    is_coord_v = is_coord<K>::value;


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    constexpr
    std::enable_if_t<is_coord_v<T>, std::size_t>
    arity (void)
    {
        return T::dimension;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    constexpr
    std::enable_if_t<std::is_arithmetic<T>::value, std::size_t>
    arity (void)
    {
        return 1;
    }


    ///////////////////////////////////////////////////////////////////////////
    // vector operators
#define ELEMENT_OP(OP)                                  \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        typename U,                                     \
        template <size_t,typename> class A,             \
        template <size_t,typename> class B,             \
        typename = std::enable_if_t<                    \
            is_coord_v<A<S,T>> && is_coord_v<B<S,U>>,   \
            void                                        \
        >                                               \
    >                                                   \
    constexpr                                           \
    auto                                                \
    operator OP (A<S,T> a, B<S,U> b)                    \
    {                                                   \
        typename coord::result<A,B>::template type<     \
            S,std::common_type_t<T,U>                   \
        > out {};                                       \
        for (size_t i = 0; i < S; ++i)                  \
            out[i] = a[i] OP b[i];                      \
        return out;                                     \
    }                                                   \
                                                        \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        typename U,                                     \
        template <size_t,typename> class A,             \
        template <size_t,typename> class B,             \
        typename = std::enable_if_t<                    \
            is_coord_v<A<S,T>> &&                       \
            is_coord_v<B<S,U>> &&                       \
            std::is_same<                               \
                std::common_type_t<T,U>, T              \
            >::value,                                   \
            void                                        \
        >                                               \
    >                                                   \
    auto&                                               \
    operator PASTE(OP,=) (A<S,T>& a, B<S,U> b)          \
    {                                                   \
        for (size_t i = 0; i < S; ++i)                  \
            a[i] PASTE(OP,=) b[i];                      \
        return a;                                       \
    }

    ELEMENT_OP(+)
    ELEMENT_OP(-)
    ELEMENT_OP(*)
    ELEMENT_OP(/)
    ELEMENT_OP(%)
#undef ELEMENT_OP

    ///////////////////////////////////////////////////////////////////////////
    // scalar operators
#define SCALAR_OP(OP)                                   \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        typename U,                                     \
        template <size_t,typename> class K,             \
        typename = std::enable_if_t<                    \
            coord::has_scalar_op_v<K>, void             \
        >                                               \
    >                                                   \
    constexpr                                           \
    auto                                                \
    operator OP (U u, K<S,T> k)                         \
    {                                                   \
        K<S,std::common_type_t<T,U>> out{};             \
                                                        \
        for (size_t i = 0; i < S; ++i)                  \
            out[i] = u OP k[i];                         \
        return out;                                     \
    }                                                   \
                                                        \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        typename U,                                     \
        template <size_t,typename> class K,             \
        typename = std::enable_if_t<                    \
            coord::has_scalar_op_v<K>,void              \
        >                                               \
    >                                                   \
    constexpr                                           \
    auto                                                \
    operator OP (K<S,T> k, U u)                         \
    {                                                   \
        K<S,std::common_type_t<T,U>> out {};            \
                                                        \
        for (size_t i = 0; i < S; ++i)                  \
            out[i] = k[i] OP u;                         \
        return out;                                     \
    }

    SCALAR_OP(+)
    SCALAR_OP(-)
    SCALAR_OP(*)
    SCALAR_OP(/)
    SCALAR_OP(%)

#undef SCALAR_OP


    //-------------------------------------------------------------------------
    // scalar assignment operators.
    //
    // we must check the operands/results do not need casting to store in the
    // destination type to avoid silent errors accumulating.
#define SCALAR_OP(OP)                                       \
    template <                                              \
        size_t S,                                           \
        typename T,                                         \
        typename U,                                         \
        template <size_t,typename> class K,                 \
        typename = std::enable_if_t<                        \
            is_coord<K<S,T>>::value &&                      \
            std::is_arithmetic<T>::value &&                 \
            std::is_arithmetic<U>::value,                   \
            void                                            \
        >                                                   \
    >                                                       \
    std::enable_if_t<                                       \
        std::is_same<                                       \
            T,                                              \
            std::common_type_t<T,U>                         \
        >::value,                                           \
        K<S,T>                                              \
    >&                                                      \
    operator OP (K<S,T> &k, U u)                            \
    {                                                       \
        for (size_t i = 0; i < S; ++i)                      \
            k[i] OP u;                                      \
                                                            \
        return k;                                           \
    }

    SCALAR_OP(+=)
    SCALAR_OP(-=)
    SCALAR_OP(*=)
    SCALAR_OP(/=)
    SCALAR_OP(%=)
#undef SCALAR_OP


    ///////////////////////////////////////////////////////////////////////////
    // unary operators

#define UNARY_OP(OP)                                \
    template <                                      \
        size_t S,                                   \
        typename T,                                 \
        template <size_t,typename> class K,         \
        typename = std::enable_if_t<                \
            is_coord_v<K<S,T>>, void                \
        >                                           \
    >                                               \
    constexpr                                       \
    auto                                            \
    operator OP (K<S,T> k)                          \
    {                                               \
        K<S,decltype(OP std::declval<T> ())> out{}; \
                                                    \
        for (size_t i = 0; i < S; ++i)              \
            out[i] = OP k[i];                       \
                                                    \
        return out;                                 \
    }

    UNARY_OP(!)
    UNARY_OP(~)
    UNARY_OP(+)
    UNARY_OP(-)

#undef UNARY_OP


    ///////////////////////////////////////////////////////////////////////////
    // logic operators

    /// elementwise equality operator
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    bool
    operator== (const K<S,T> a, const K<S,T> b)
    {
        bool (*predicate)(const T&, const T&) = almost_equal;

        return std::equal (std::cbegin (a),
                           std::cend   (a),
                           std::cbegin (b),
                           predicate);
    }

    ///------------------------------------------------------------------------
    /// elementwise inquality operator
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    bool
    operator!= (K<S,T> a, K<S,T> b)
    {
        return !(a == b);
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    bool
    almost_zero (const K<S,T> &k)
    {
        return std::all_of (
            std::cbegin (k),
            std::cend (k),
            [] (T t) { return almost_equal (t); }
        );
    }


    ///////////////////////////////////////////////////////////////////////////
    // special operators

    /// point-point subtraction giving a vector difference
    template <
        size_t S,
        typename T,
        typename U
    >
    constexpr
    vector<S,std::common_type_t<T,U>>
    operator- (point<S,T> a, point<S,U> b)
    {
        vector<S,std::common_type_t<T,U>> out {};
        for (size_t i = 0; i < S; ++i)
            out[i] = a[i] - b[i];
        return out;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        typename U,
        typename = std::enable_if_t<
            std::is_arithmetic<T>::value && std::is_arithmetic<U>::value,
            void
	>
    >
    constexpr
    vector<S,std::common_type_t<T,U>>
    operator- (U u, point<S,T> p)
    {
        return point<S,U> {u} - p;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T
    >
    constexpr
    T
    dot (const T (&a)[S], const T (&b)[S])
    {
        T sum = 0;
        for (size_t i = 0; i < S; ++i)
            sum += a[i] * b[i];
        return sum;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class A,
        template <size_t,typename> class B,
        typename = std::enable_if_t<
            is_coord_v<A<S,T>> && is_coord_v<B<S,T>>, void
        >
    >
    constexpr
    T
    dot (A<S,T> a, B<S,T> b)
    {
        return dot<S,T> (a.data, b.data);
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    T
    dot (K<S,T> a, const T (&b)[S])
    {
        return dot<S,T> (a.data, b);
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    T
    dot (const T (&a)[S], K<S,T> b)
    {
        return dot<S,T> (a, b.data);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<coord::has_norm_v<K>,void>
    >
    constexpr
    T
    norm2 (const K<S,T> &k)
    {
        return dot (k, k);
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            coord::has_norm_v<K>,
            void
        >
    >
    constexpr
    T
    norm (const K<S,T> &k)
    {
        return std::sqrt (norm2 (k));
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            coord::has_norm_v<K>,
            void
        >
    >
    constexpr
    K<S,T>
    normalised (const K<S,T> &k)
    {
        auto n = norm (k);
        CHECK_NEZ (n);

        return k / n;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            coord::has_norm_v<K>,
            void
        >
    >
    constexpr
    bool
    is_normalised (const K<S,T> &k)
    {
        return almost_equal (norm2 (k), T{1});
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    K<S,T>
    abs (K<S,T> k)
    {
        for (auto &v: k)
            v = std::abs (v);
        return k;
    }

    ///////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    K<S,T>
    pow (K<S,T> k)
    {
        for (auto &v: k)
            v = pow (v);
        return k;
    }

    ///////////////////////////////////////////////////////////////////////////
    // root of sum of squares
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    T
    hypot (K<S,T> k)
    {
        return std::sqrt (sum (k * k));
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    K<S,T>
    mod (K<S,T> k, T t)
    {
        std::transform (
            std::cbegin (k),
            std::cend   (k),
            std::begin  (k),
            [t] (auto v) { return mod (v, t);
        });
        return k;
    }

    ///////////////////////////////////////////////////////////////////////////
    // trigonometric functions
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<is_coord_v<K<S,T>>,void>
    >
    constexpr
    K<S,T>
    sin (K<S,T> k)
    {
        std::transform (
            std::cbegin (k),
            std::cend   (k),
            std::begin  (k),
            [] (auto v) { return std::sin (v); }
        );

        return k;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<is_coord_v<K<S,T>>,void>
    >
    constexpr
    K<S,T>
    cos (K<S,T> k)
    {
        std::transform (
            std::cbegin (k),
            std::cend   (k),
            std::begin  (k),
            [] (auto v) { return std::cos (v); }
        );

        return k;
    }


    ///////////////////////////////////////////////////////////////////////////
    // logical element operators

    /// return a coord type containing the max element at each offset
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    K<S,T>
    min (K<S,T> a, K<S,T> b)
    {
        K<S,T> out {};
        for (size_t i = 0; i < S; ++i)
            out[i] = min (a[i], b[i]);
        return out;
    }


    ///------------------------------------------------------------------------
    // /return a coord type containing the max element at each offset
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    K<S,T>
    max (K<S,T> a, K<S,T> b)
    {
        K<S,T> out {};
        for (size_t i = 0; i < S; ++i)
            out[i] = max (a[i], b[i]);
        return out;
    }


    ///------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template<size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    T
    min (const K<S,T> k)
    {
        return *std::min_element (std::cbegin (k), std::cend (k));
    }


    template <
        size_t S,
        typename T,
        template<size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    T
    max (const K<S,T> k)
    {
        return *std::max_element (std::cbegin (k), std::cend (k));
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    T
    sum (const K<S,T> k)
    {
        // DO NOT USE util::sum(begin, end) from maths.hpp
        //
        // It would be nice to use kahan summation from maths.hpp but speed
        // and simplicity is more important for these fixed sized
        // coordinates. Infinities tend to crop up using these classes and
        // they cause a few headaches in the kahan code.
        //
        // So, if the user wants kahan summation they can request it
        // explicitly.

        return std::accumulate (std::cbegin (k), std::cend (k), T{0});
    }


    ///////////////////////////////////////////////////////////////////////////
#define VECTOR_OP(OP)                                       \
    template <                                              \
        size_t S,                                           \
        typename T,                                         \
        typename U,                                         \
        template <size_t,typename> class A,                 \
        template <size_t,typename> class B,                 \
        typename = std::enable_if_t<                        \
            is_coord_v<A<S,T>> && is_coord_v<B<S,U>>, void  \
        >                                                   \
    >                                                       \
    constexpr                                               \
    vector<S,bool>                                          \
    operator OP (const A<S,T> a, const B<S,U> b)            \
    {                                                       \
        vector<S,bool> out {};                              \
        for (size_t i = 0; i < S; ++i)                      \
            out[i] = a[i] OP b[i];                          \
        return out;                                         \
    }

    VECTOR_OP(<)
    VECTOR_OP(>)
    VECTOR_OP(<=)
    VECTOR_OP(>=)
    VECTOR_OP(&&)
    VECTOR_OP(||)

#undef VECTOR_OP


#define SCALAR_OP(OP)                       \
    template <                              \
        size_t S,                           \
        typename T,                         \
        typename U,                         \
        template <size_t,typename> class K, \
        typename = std::enable_if_t<        \
            is_coord_v<K<S,T>>, void        \
        >                                   \
    >                                       \
    constexpr                               \
    vector<S,bool>                          \
    operator OP (const K<S,T> k, const U u) \
    {                                       \
        vector<S,bool> out {};              \
        for (size_t i = 0; i < S; ++i)      \
            out[i] = k[i] OP u;             \
        return out;                         \
    }

    SCALAR_OP(<)
    SCALAR_OP(>)
    SCALAR_OP(<=)
    SCALAR_OP(>=)
    SCALAR_OP(&&)
    SCALAR_OP(||)

#undef SCALAR_OP


    //-------------------------------------------------------------------------
    template <
        size_t S,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,bool>>, void
        >
    >
    constexpr
    bool
    any (const K<S,bool> k)
    {
        return std::any_of (std::cbegin (k),
                            std::cend   (k),
                            identity<bool>);
    }

    //-------------------------------------------------------------------------
    template <
        size_t S,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,bool>>, void
        >
    >
    constexpr
    bool
    all (const K<S,bool> k)
    {
        return std::all_of (std::cbegin (k),
                            std::cend   (k),
                            identity<bool>);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T,
        template<size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>> && std::is_floating_point<T>::value, void
        >
    >
    constexpr
    K<S,T>
    floor (const K<S,T> k)
    {
        T (*floor_func)(T) = std::floor;

        K<S,T> out {};
        std::transform (std::cbegin (k),
                        std::cend   (k),
                        std::begin  (out),
                        floor_func);
        return out;
    }

    /// returns the data at a templated index in a coordinate.
    ///
    /// specifically required for structured bindings support.
    ///
    /// \tparam I index of the requested data
    /// \tparam S dimensionality of the coordinate
    /// \tparam T underlying data type of the coordinate
    /// \tparam K coordinate data type to operate on
    template <
        size_t I,
        size_t S,
        typename T,
        template<
        size_t,
        typename
    > class K
    >
    const std::enable_if_t<
        is_coord_v<K<S,T>>,
        T
    >&
    get (const K<S,T> &k)
    {
        static_assert (I < S);
        return k[I];
    };


    /// returns the data at a templated index in a coordinate.
    ///
    /// specifically required for structured bindings support.
    ///
    /// \tparam I index of the requested data
    /// \tparam S dimensionality of the coordinate
    /// \tparam T underlying data type of the coordinate
    /// \tparam K coordinate data type to operate on
    template <
        size_t I,
        size_t S,
        typename T,
        template<
            size_t,
            typename
        > class K
    >
    std::enable_if_t<
        is_coord_v<K<S,T>>,
        T
    >&
    get (K<S,T> &k)
    {
        static_assert (I < S);
        return k[I];
    };
}


///////////////////////////////////////////////////////////////////////////////
#include <tuple>

namespace std {
    /// returns the dimensions of a coordinate type.
    ///
    /// specifically required for structured bindings support.
    ///
    /// \tparam S dimensions
    /// \tparam T data type
    /// \tparam K coordinate class
    template <
        size_t S,
        typename T,
        template<
            size_t,
            typename
        > class K
    >
    class tuple_size<K<S,T>> : public std::enable_if_t<
        ::util::is_coord_v<K<S,T>>,
        std::integral_constant<decltype(S), S>
    > { };


    /// indicates the type at a given index of a coordinate type
    ///
    /// specifically required for structured bindings support.
    ///
    /// \tparam I data index
    /// \tparam S dimensionality of the coordinate
    /// \tparam T data type for the coordinate
    /// \tparam K the underlying coordinate class
    template <
        size_t I,
        size_t S,
        typename T,
        template<
            size_t,
            typename
        > class K
    >
    class tuple_element<
        I, K<S,T>
    > : public enable_if<
        ::util::is_coord_v<K<S,T>>,
        T
    > { };
}


///////////////////////////////////////////////////////////////////////////////
#include <functional>

#include "../hash.hpp"

namespace std {
    template <
        size_t S,
        typename T,
        template <
            std::size_t,typename
        > class K
    >
    struct hash<
        K<S,T>
    > : public ::std::enable_if<
        ::util::is_coord_v<K<S,T>>
    > {
        std::size_t
        operator() (K<S,T> k) const {
            size_t v = 0xdeadbeef;

            for (auto t: k)
                v = ::util::hash::mix (t, v);

            return v;
        }
    };
}


#endif
