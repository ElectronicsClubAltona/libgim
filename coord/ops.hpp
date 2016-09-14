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

#include "../preprocessor.hpp"
#include "../maths.hpp"
#include "../types/bits.hpp"

#include <cmath>
#include <cstdlib>

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
        template <> struct has_norm<quaternion> : public std::true_type { };

        template <template <size_t,typename> class K>
        constexpr auto has_norm_v = has_norm<K>::value;


        //---------------------------------------------------------------------
        template <template <size_t,typename> class K>
        struct has_scalar_op : public std::false_type { };

        template <> struct has_scalar_op<colour>     : public std::true_type { };
        template <> struct has_scalar_op<extent>     : public std::true_type { };
        template <> struct has_scalar_op<point>      : public std::true_type { };
        template <> struct has_scalar_op<quaternion> : public std::true_type { };
        template <> struct has_scalar_op<vector>     : public std::true_type { };

        template <template <size_t,typename> class K>
        constexpr auto has_scalar_op_v = has_scalar_op<K>::value;
    }

    template <template <size_t,typename> class> struct is_coord : std::false_type { };

    template <> struct is_coord<point>      : std::true_type { };
    template <> struct is_coord<extent>     : std::true_type { };
    template <> struct is_coord<vector>     : std::true_type { };
    template <> struct is_coord<colour>     : std::true_type { };
    template <> struct is_coord<quaternion> : std::true_type { };

    template <template <size_t,typename> class K>
    constexpr bool
    is_coord_v = is_coord<K>::value;

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
            is_coord_v<A> && is_coord_v<B>,             \
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
            is_coord_v<A> &&                            \
            is_coord_v<B> &&                            \
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
            is_coord<K>::value &&                           \
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
            is_coord_v<K>, void                     \
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
            is_coord_v<K>, void
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
            is_coord_v<K>, void
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
            is_coord_v<K>, void
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
            is_coord_v<A> && is_coord_v<B>, void
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
            is_coord_v<K>, void
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
            is_coord_v<K>, void
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
        T sum = T{0};
        for (auto &t: k)
            sum += t * t;
        return sum;
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
        return k / norm (k);
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
            is_coord_v<K>, void
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
            is_coord_v<K>, void
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
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr
    K<S,T>
    mod (K<S,T> k, T t)
    {
        std::transform (std::cbegin (k), std::cend (k), std::begin (k), [t] (auto v) { return mod (v, t); });
        return k;
    }

    ///////////////////////////////////////////////////////////////////////////
    // trigonometric functions
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K,
        typename = std::enable_if_t<is_coord_v<K>,void>
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
        typename = std::enable_if_t<is_coord_v<K>,void>
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
            is_coord_v<K>, void
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
            is_coord_v<K>, void
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
            is_coord_v<K>, void
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
            is_coord_v<K>, void
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
            is_coord_v<K>, void
        >
    >
    constexpr
    T
    sum (const K<S,T> k)
    {
        return sum (std::cbegin (k), std::cend (k));
    }


    ///////////////////////////////////////////////////////////////////////////
#define VECTOR_OP(OP)                               \
    template <                                      \
        size_t S,                                   \
        typename T,                                 \
        typename U,                                 \
        template <size_t,typename> class A,         \
        template <size_t,typename> class B,         \
        typename = std::enable_if_t<                \
            is_coord_v<A> && is_coord_v<B>, void    \
        >                                           \
    >                                               \
    constexpr                                       \
    vector<S,bool>                                  \
    operator OP (const A<S,T> a, const B<S,U> b)    \
    {                                               \
        vector<S,bool> out {};                      \
        for (size_t i = 0; i < S; ++i)              \
            out[i] = a[i] OP b[i];                  \
        return out;                                 \
    }

    VECTOR_OP(<)
    VECTOR_OP(>)
    VECTOR_OP(<=)
    VECTOR_OP(>=)

#undef VECTOR_OP


#define SCALAR_OP(OP)                       \
    template <                              \
        size_t S,                           \
        typename T,                         \
        typename U,                         \
        template <size_t,typename> class K, \
        typename = std::enable_if_t<        \
            is_coord_v<K>, void             \
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

#undef SCALAR_OP


    //-------------------------------------------------------------------------
    template <
        size_t S,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr
    bool
    any (const K<S,bool> k)
    {
        return std::any_of (std::cbegin (k),
                            std::cbegin (k),
                            identity<bool>);
    }

    //-------------------------------------------------------------------------
    template <
        size_t S,
        template <size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr
    bool
    all (const K<S,bool> k)
    {
        return std::all_of (std::cbegin (k),
                            std::cbegin (k),
                            identity<bool>);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        size_t S,
        typename T,
        template<size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K> && std::is_floating_point<T>::value, void
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
}

#endif
