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

#ifndef __UTIL_COORDS_OPS
#define __UTIL_COORDS_OPS

#include "fwd.hpp"
#include "traits.hpp"

// we specifically rely on vector<bool> to compute a few logical operations
#include "../vector.hpp"

#include "../debug.hpp"
#include "../maths.hpp"
#include "../preprocessor.hpp"
#include "../types/bits.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <functional>

namespace util {
    ///////////////////////////////////////////////////////////////////////////
    // a templated functor that exposes arithmetic and assignment maths
    // functions for vector-vector or vector-scalar operations.
    //
    // we implement the operations this way because it (somewhat) simplifies
    // ambiguity resolution in the various operators we need to provide.
    // eg, operator+(vec,vec) vs operator+(vec,int).
    //
    // it used to be directly implemented with a series of templated free
    // functions when we could restrict the arguments more easily with quite
    // specific template template parameters. but the introduction of
    // coordinate types that do not expose size or type information as template
    // parameters we can't rely on this mechanism anymore.
    template <typename, typename,typename=void>
    struct ops;


    //-------------------------------------------------------------------------
    // vector operators
    template <typename ValueA, typename ValueB>
    struct ops<
        ValueA,
        ValueB,
        std::enable_if_t<
            is_coord_v<ValueA> &&
            is_coord_v<ValueB> &&
            arity<ValueA>::value == arity<ValueB>::value &&
            std::is_same_v<
                typename ValueA::value_type,
                typename ValueB::value_type
            >,
            void
        >
    > {
        template <typename OpT>
        static constexpr auto
        arithmetic (OpT op, ValueA a, ValueB b)
        {
            result_t<ValueA,ValueB> out {};
            for (std::size_t i = 0; i < ValueA::elements; ++i)
                out[i] = op (a[i], b[i]);
            return out;
        }

        template <typename OpT>
        static constexpr ValueA&
        assignment (OpT op, ValueA &a, ValueB b)
        {
            for (std::size_t i = 0; i < ValueA::elements; ++i)
                a[i] = op (a[i], b[i]);
            return a;
        }
    };


    ///////////////////////////////////////////////////////////////////////////
    /// create a coord from supplied arguments, optionally specifying the
    /// underlying type.
    ///
    /// much like experimental::make_array we use a void type to signal we
    /// need to deduce the underlying type.
#define MAKE_COORD(KLASS)                   \
    template <                              \
        typename _T = void,                 \
        typename ...Args                    \
    >                                       \
    constexpr auto                          \
    make_##KLASS (Args &&...args)           \
    {                                       \
        using T = std::conditional_t<       \
            std::is_void_v<_T>,             \
            std::common_type_t<Args...>,    \
            _T                              \
        >;                                  \
                                            \
        return KLASS<sizeof...(Args),T> {   \
            std::forward<Args> (args)...    \
        };                                  \
    }

    MAKE_COORD(extent)
    MAKE_COORD(point)
    MAKE_COORD(vector)

#undef MAKE_COORD

    template <
        template <std::size_t,typename> class K,
        typename ...Args
    >
    constexpr auto
    make_coord (Args &&...args)
    {
        using T = std::common_type_t<Args...>;
        return K<sizeof...(Args),T> { std::forward<Args> (args)... };
    }


    //-------------------------------------------------------------------------
    // vector-scalar operations
    // 
    // we allow scalar types which can be naturally promoted to the vector's
    // value_type
    template <
        typename CoordT,
        typename ScalarT
    >
    struct ops<
        CoordT,
        ScalarT,
        std::enable_if_t<
            has_scalar_op_v<CoordT> &&
            is_coord_v<CoordT> &&
            std::is_same_v<
                typename CoordT::value_type,
                std::common_type_t<
                    typename CoordT::value_type,
                    ScalarT
                >
            >,
            void
        >
    > {
        template <typename OpT>
        static constexpr CoordT&
        assignment (OpT &&op, CoordT &c, ScalarT s)
        {
            for (size_t i = 0; i < CoordT::elements; ++i)
                c[i] = op (c[i], s);
            return c;
        }


        template <typename OpT>
        static constexpr CoordT
        arithmetic (OpT &&op, CoordT c, ScalarT s)
        {
            CoordT out {};
            for (size_t i = 0; i < CoordT::elements; ++i)
                out[i] = op (c[i], s);
            return out;
        }
    };


    //-------------------------------------------------------------------------
    // scalar-vector operations
    template <typename ScalarT, typename CoordT>
    struct ops<
        ScalarT,
        CoordT,
        std::enable_if_t<
            has_scalar_op_v<CoordT> &&
            std::is_arithmetic_v<ScalarT> &&
            std::is_same_v<
                std::common_type_t<ScalarT, typename CoordT::value_type>,
                typename CoordT::value_type
            >,
            void
        >
    > {
        template <typename OpT>
        static constexpr CoordT
        arithmetic (OpT op, const ScalarT &s, const CoordT &c) noexcept
        {
            CoordT out {};
            for (size_t i = 0; i < CoordT::elements; ++i)
                out[i] = op (s, c[i]);
            return out;
        }
    };


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename A,
        typename B,
        typename = std::enable_if_t<
            (is_coord_v<std::decay_t<A>> || is_coord_v<std::decay_t<B>>) &&
            (is_coord_v<std::decay_t<A>> || std::is_arithmetic_v<std::decay_t<A>>) &&
            (is_coord_v<std::decay_t<B>> || std::is_arithmetic_v<std::decay_t<B>>)
        >
    >
    constexpr auto
    operator+ (A &&a, B &&b)
    {
        return ops<
            std::decay_t<A>,
            std::decay_t<B>
        >::template arithmetic (std::plus{}, a, b);
    }


    template <
        typename A,
        typename B,
        typename = std::enable_if_t<
            (is_coord_v<std::decay_t<A>> || is_coord_v<std::decay_t<B>>) &&
            (is_coord_v<std::decay_t<A>> || std::is_arithmetic_v<std::decay_t<A>>) &&
            (is_coord_v<std::decay_t<B>> || std::is_arithmetic_v<std::decay_t<B>>)
        >
    >
    constexpr auto
    operator- (A &&a, B &&b)
    {
        return ops<
            std::decay_t<A>,
            std::decay_t<B>
        >::template arithmetic (std::minus{}, a, b);
    }


    template <
        typename A,
        typename B,
        typename = std::enable_if_t<
            (is_coord_v<std::decay_t<A>> || is_coord_v<std::decay_t<B>>) &&
            (is_coord_v<std::decay_t<A>> || std::is_arithmetic_v<std::decay_t<A>>) &&
            (is_coord_v<std::decay_t<B>> || std::is_arithmetic_v<std::decay_t<B>>)
        >
    >
    constexpr auto
    operator* (A &&a, B &&b)
    {
        return ops<
            std::decay_t<A>,
            std::decay_t<B>
        >::template arithmetic (std::multiplies{}, a, b);
    }


    template <
        typename A,
        typename B,
        typename = std::enable_if_t<
            (is_coord_v<std::decay_t<A>> || is_coord_v<std::decay_t<B>>) &&
            (is_coord_v<std::decay_t<A>> || std::is_arithmetic_v<std::decay_t<A>>) &&
            (is_coord_v<std::decay_t<B>> || std::is_arithmetic_v<std::decay_t<B>>)
        >
    >
    constexpr auto
    operator/ (A &&a, B &&b)
    {
        return ops<
            std::decay_t<A>,
            std::decay_t<B>
        >::template arithmetic (std::divides{}, a, b);
    }


    //-------------------------------------------------------------------------
    template <
        typename A,
        typename B,
        typename = std::enable_if_t<
            (is_coord_v<std::decay_t<A>> || is_coord_v<std::decay_t<B>>) &&
            (is_coord_v<std::decay_t<A>> || std::is_arithmetic_v<std::decay_t<A>>) &&
            (is_coord_v<std::decay_t<B>> || std::is_arithmetic_v<std::decay_t<B>>)
        >
    >
    constexpr auto
    operator += (A &&a, B &&b)
    {
        return ops<
            std::decay_t<A>,
            std::decay_t<B>
        >::template assignment (std::plus{}, a, b);
    }


    template <
        typename A,
        typename B,
        typename = std::enable_if_t<
            (is_coord_v<std::decay_t<A>> || is_coord_v<std::decay_t<B>>) &&
            (is_coord_v<std::decay_t<A>> || std::is_arithmetic_v<std::decay_t<A>>) &&
            (is_coord_v<std::decay_t<B>> || std::is_arithmetic_v<std::decay_t<B>>)
        >
    >
    constexpr auto
    operator -= (A &&a, B &&b)
    {
        return ops<
            std::decay_t<A>,
            std::decay_t<B>
        >::template assignment (std::minus{}, a, b);
    }


    template <
        typename A,
        typename B,
        typename = std::enable_if_t<
            (is_coord_v<std::decay_t<A>> || is_coord_v<std::decay_t<B>>) &&
            (is_coord_v<std::decay_t<A>> || std::is_arithmetic_v<std::decay_t<A>>) &&
            (is_coord_v<std::decay_t<B>> || std::is_arithmetic_v<std::decay_t<B>>)
        >
    >
    constexpr auto
    operator *= (A &&a, B &&b)
    {
        return ops<
            std::decay_t<A>,
            std::decay_t<B>
        >::template assignment (std::multiplies{}, a, b);
    }


    template <
        typename A,
        typename B,
        typename = std::enable_if_t<
            (is_coord_v<std::decay_t<A>> || is_coord_v<std::decay_t<B>>) &&
            (is_coord_v<std::decay_t<A>> || std::is_arithmetic_v<std::decay_t<A>>) &&
            (is_coord_v<std::decay_t<B>> || std::is_arithmetic_v<std::decay_t<B>>)
        >
    >
    constexpr auto
    operator /= (A &&a, B &&b)
    {
        return ops<
            std::decay_t<A>,
            std::decay_t<B>
        >::template assignment (std::divides{}, a, b);
    }


    ///////////////////////////////////////////////////////////////////////////
    // unary operators
#define UNARY_OP(OP)                                    \
    template <                                          \
        typename K,                                     \
        typename = std::enable_if_t<                    \
            is_coord_v<K>, void                         \
        >                                               \
    >                                                   \
    constexpr                                           \
    auto                                                \
    operator OP (K k)                                   \
    {                                                   \
        using value_type = decltype(                    \
            OP std::declval<typename K::value_type> ()  \
        );                                              \
                                                        \
        revalue_t<K,value_type> out {};                 \
                                                        \
        for (std::size_t i = 0; i < K::elements; ++i)   \
            out[i] = OP k[i];                           \
                                                        \
        return out;                                     \
    }

    UNARY_OP(!)
    UNARY_OP(~)
    UNARY_OP(+)
    UNARY_OP(-)

#undef UNARY_OP


    ///////////////////////////////////////////////////////////////////////////
    // logic operators
    namespace detail {
        template <
            typename RetT,
            typename ArgT,
            typename FuncT,
            std::size_t ...Indices,
            typename = std::enable_if_t<
                is_coord_v<ArgT> && is_coord_v<RetT>, void
            >
        >
        constexpr auto
        apply (const std::index_sequence<Indices...>,
               FuncT &&func,
               const ArgT &a,
               const ArgT &b)
        {
            return RetT {
                std::invoke (func, a[Indices], b[Indices])...
            };
        }
    }


    template <
        typename RetT,
        std::size_t S,
        typename T,
        template <std::size_t,typename> class ArgT,
        typename FuncT,
        typename = std::enable_if_t<
            is_coord_v<RetT> && is_coord_v<ArgT>, void
        >,
        typename Indices = std::make_index_sequence<S>
    >
    constexpr auto
    apply (FuncT &&func, const ArgT<S,T> &a, const ArgT<S,T> &b)
    {
        return detail::apply (Indices{}, std::forward<FuncT> (func), a, b);
    }


    ///////////////////////////////////////////////////////////////////////////
    // logic operators
    namespace detail {
        template <
            typename K,
            typename FuncT,
            typename = std::enable_if_t<
                is_coord_v<K>, void
            >,
            std::size_t ...Indices
        >
        constexpr auto
        compare (FuncT &&func, std::index_sequence<Indices...>, const K a, const K b)
        {
            return vector<K::elements,bool> {
                std::invoke (func, a[Indices], b[Indices])...
            };
        }
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename FuncT,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >,
        typename Indices = std::make_index_sequence<K::elements>
    >
    constexpr auto
    compare (const K a, const K b, FuncT &&func)
    {
        return detail::compare (std::forward<FuncT> (func), Indices{}, a, b);
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    compare (const K a, const K b)
    {
        return compare (a, b, std::equal_to<typename K::value_type> {});
    }


    /// elementwise equality operator
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr bool
    operator== (const K a, const K b)
    {
        return all (compare (a, b, std::equal_to<typename K::value_type> {}));
    }

    ///------------------------------------------------------------------------
    /// elementwise inquality operator
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr bool
    operator!= (const K a, const K b)
    {
        return any (compare (a, b, std::not_equal_to<typename K::value_type> {}));
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr
    bool
    almost_zero (const K &k)
    {
        return std::all_of (
            std::cbegin (k),
            std::cend (k),
            [] (auto t) { return almost_zero (t); }
        );
    }


    ///////////////////////////////////////////////////////////////////////////
    // special operators

    /// point-point subtraction giving a vector difference
    template <
        std::size_t S,
        typename T,
        typename U
    >
    constexpr
    vector<S,std::common_type_t<T,U>>
    operator- (point<S,T> a, point<S,U> b)
    {
        vector<S,std::common_type_t<T,U>> out {};
        for (std::size_t i = 0; i < S; ++i)
            out[i] = a[i] - b[i];
        return out;
    }


    //-------------------------------------------------------------------------
    template <
        std::size_t S,
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
        std::size_t S,
        typename T
    >
    constexpr
    T
    dot (const T (&a)[S], const T (&b)[S])
    {
        T sum = 0;
        for (std::size_t i = 0; i < S; ++i)
            sum += a[i] * b[i];
        return sum;
    }


    template <
        std::size_t S,
        typename T,
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K> && std::is_same_v<typename K::value_type, T> && K::elements == S,
            void
        >
    >
    constexpr
    T
    dot (const T (&a)[S], K k)
    {
        return dot (a, k.data);
    }


    //-------------------------------------------------------------------------
    template <
        std::size_t S,
        typename T,
        template <std::size_t,typename> class A,
        template <std::size_t,typename> class B,
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
        typename K,
        typename T,
        typename = std::enable_if_t<
            is_coord_v<K> && std::is_same_v<T,typename K::value_type>, void
        >
    >
    constexpr auto
    dot (K a, const T (&b)[K::elements])
    {
        return dot (a.data, b);
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    dot (const typename K::value_type (&a)[K::elements], K b)
    {
        return dot (a, b.data);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename K,
        typename = std::enable_if_t<has_norm_v<K>,void>
    >
    constexpr
    auto
    norm2 (const K &k)
    {
        return dot (k, k);
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            has_norm_v<K>,
            void
        >
    >
    constexpr
    auto
    norm (const K &k)
    {
        return std::sqrt (norm2 (k));
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            has_norm_v<K>,
            void
        >
    >
    constexpr
    auto
    normalised (const K &k)
    {
        CHECK_NEZ (norm (k));
        return k / norm (k);
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            has_norm_v<K>,
            void
        >
    >
    constexpr
    bool
    is_normalised (const K &k)
    {
        return almost_equal (norm2 (k), 1);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr
    K
    abs (K k)
    {
        for (auto &v: k)
            v = std::abs (v);
        return k;
    }

    ///////////////////////////////////////////////////////////////////////////
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr
    K
    pow (K k)
    {
        for (auto &v: k)
            v = pow (v);
        return k;
    }

    ///////////////////////////////////////////////////////////////////////////
    // root of sum of squares
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr typename K::value_type
    hypot (K k)
    {
        return std::sqrt (sum (k * k));
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename K,
        typename T,
        typename = std::enable_if_t<
            is_coord_v<K> && std::is_same_v<T, typename K::value_type>, void
        >
    >
    constexpr auto
    mod (K k, T t)
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
        typename K,
        typename = std::enable_if_t<is_coord_v<K>,void>
    >
    constexpr auto
    sin (K k)
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
        typename K,
        typename = std::enable_if_t<is_coord_v<K>,void>
    >
    constexpr auto
    cos (K k)
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
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >,
        typename ...Args
    >
    constexpr auto
    min (K a, K b, Args &&...args)
    {
        // the varargs must be the same types as the first two arguments
        static_assert ((
            ... && std::is_same_v<
                K,
                std::decay_t<Args>
            >
        ));

        K out {};
        for (std::size_t i = 0; i < K::elements; ++i)
            out[i] = min (a[i], b[i], args[i]...);
        return out;
    }


    ///------------------------------------------------------------------------
    // /return a coord type containing the max element at each offset
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >,
        typename ...Args
    >
    constexpr auto
    max (K a, K b, Args &&...args)
    {
        static_assert ((
            ... && std::is_same_v<
                K,
                std::decay_t<Args>
            >
        ));

        K out {};
        for (std::size_t i = 0; i < K::elements; ++i)
            out[i] = max (a[i], b[i], args[i]...);
        return out;
    }


    //-------------------------------------------------------------------------
    /// returns a coordinate type where each element has been clamped to the
    /// range [lo,hi].
    ///
    /// we specifically do not allow different coordinate types for val, lo,
    /// and hi because the min and max calls are ill definied for varying
    /// types (not because varying types would not be useful).
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    limit (K k, K lo, K hi)
    {
        assert (all (lo <= hi));
        return max (min (k, hi), lo);
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    limit (K k, typename K::value_type lo, K hi)
    {
        return limit (k, K {lo}, hi);
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    limit (K k, K lo, typename K::value_type hi)
    {
        return limit (k, lo, K {hi});
    }


    //-------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    limit (K k, typename K::value_type lo, typename K::value_type hi)
    {
        return limit (k, K {lo}, K {hi});
    }


    ///------------------------------------------------------------------------
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    min (const K &k)
    {
        return *std::min_element (std::cbegin (k), std::cend (k));
    }


    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    max (const K &k)
    {
        return *std::max_element (std::cbegin (k), std::cend (k));
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    sum (const K &k)
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

        return std::accumulate (std::cbegin (k), std::cend (k), typename K::value_type{0});
    }


    ///////////////////////////////////////////////////////////////////////////
#define VECTOR_OP(OP)                                   \
    template <                                          \
        typename A,                                     \
        typename B,                                     \
        typename = std::enable_if_t<                    \
            is_coord_v<A> &&                            \
            is_coord_v<B> &&                            \
            A::elements == B::elements &&               \
            std::is_same_v<                             \
                typename A::value_type,                 \
                typename B::value_type                  \
            >,                                          \
            void                                        \
        >                                               \
    >                                                   \
    constexpr auto                                      \
    operator OP (const A a, const B b)                  \
    {                                                   \
        vector<A::elements,bool> out {};                \
        for (std::size_t i = 0; i < A::elements; ++i)   \
            out[i] = a[i] OP b[i];                      \
        return out;                                     \
    }

    VECTOR_OP(<)
    VECTOR_OP(>)
    VECTOR_OP(<=)
    VECTOR_OP(>=)
    VECTOR_OP(&&)
    VECTOR_OP(||)

#undef VECTOR_OP


#define SCALAR_OP(OP)                                   \
    template <                                          \
        typename K,                                     \
        typename U,                                     \
        typename = std::enable_if_t<                    \
            is_coord_v<K> &&                            \
            std::is_arithmetic_v<U>,                    \
            void                                        \
        >                                               \
    >                                                   \
    constexpr auto                                      \
    operator OP (const K &k, const U u)                 \
    {                                                   \
        vector<K::elements,bool> out {};                \
        for (std::size_t i = 0; i < K::elements; ++i)   \
            out[i] = k[i] OP u;                         \
        return out;                                     \
    }                                                   \
                                                        \
    template <                                          \
        typename K,                                     \
        typename U,                                     \
        typename = std::enable_if_t<                    \
            is_coord_v<K> &&                            \
            std::is_arithmetic_v<U>,                    \
            void                                        \
        >                                               \
    >                                                   \
    constexpr auto                                      \
    operator OP (const U u, const K &k)                 \
    {                                                   \
        vector<K::elements,bool> out {};                \
        for (std::size_t i = 0; i < K::elements; ++i)   \
            out[i] = u OP k[i];                         \
        return out;                                     \
    }

    SCALAR_OP(<)
    SCALAR_OP(>)
    SCALAR_OP(<=)
    SCALAR_OP(>=)
    SCALAR_OP(==)
    SCALAR_OP(&&)
    SCALAR_OP(||)

#undef SCALAR_OP


    ///////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <
            std::size_t S,
            template <std::size_t,typename> class K,
            std::size_t ...I,
            typename = std::enable_if_t<
                is_coord_v<K<S,bool>>,
                void
            >
        >
        constexpr bool
        any (const K<S,bool> k, std::index_sequence<I...>)
        {
            return (k[I] || ...);
        }
    };


    ///---------------------------------------------------------------------------
    /// returns true if any element is true.
    ///
    /// this function must be suitable for use in static_assert, so it must remain
    /// constexpr.
    ///
    /// we would ideally use std::any_of, but it is not constexpr.
    /// we would ideally use range-for, but cbegin is not constexpr.
    /// so... moar templates.
    template <
        std::size_t S,
        template <std::size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,bool>>, void
        >,
        typename Indices = std::make_index_sequence<S>
    >
    constexpr
    bool
    any (const K<S,bool> k)
    {
        return detail::any (k, Indices{});
    }


    ///////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <
            std::size_t S,
            template <std::size_t,typename> class K,
            std::size_t ...I,
            typename = std::enable_if_t<
                is_coord_v<K<S,bool>>,
                void
            >
        >
        constexpr bool
        all (const K<S,bool> k, std::index_sequence<I...>)
        {
            return (k[I] && ...);
        }
    }

    //-------------------------------------------------------------------------
    /// returns true if all elements are true.
    ///
    /// this function must be suitable for use in static_assert, so it must be
    /// constexpr.
    ///
    /// we would ideally use std::all_of, but it is not constexpr.
    /// we would ideally use range-for, but cbegin is not constexpr.
    /// so... moar templates.
    template <
        std::size_t S,
        template <std::size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,bool>>, void
        >,
        typename Indices = std::make_index_sequence<S>
    >
    constexpr
    bool
    all (const K<S,bool> k)
    {
        return detail::all (k, Indices {});
    }


    ///------------------------------------------------------------------------
    /// returns an instance of K elementwise using a when s is true, and b
    /// otherwise. ie, k[i] = s[i] ? a[i] : b[i];
    ///
    /// corresponds to the function `select' from OpenCL.
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    select (vector<K::elements,bool> s, K a, K b)
    {
        K k {};
        for (std::size_t i = 0; i < K::elements; ++i)
            k[i] = s[i] ? a[i] : b[i];
        return k;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K> && std::is_floating_point<typename K::value_type>::value, void
        >
    >
    constexpr auto
    floor (const K &k)
    {
        using value_type = typename K::value_type;
        value_type (*floor_func)(value_type) = std::floor;

        K out {};
        std::transform (std::cbegin (k),
                        std::cend   (k),
                        std::begin  (out),
                        floor_func);
        return out;
    }


    ///////////////////////////////////////////////////////////////////////////
    /// shifts all elements `num' indices to the right, setting the left-most
    /// `num' indices to the value `fill'.
    ///
    /// num must be between 0 and S. when 0 it is equivalent to an ordinary
    /// fill, when S it is equivalent to a noop.
    template<
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    rshift (const K k, const int num, const K fill)
    {
        CHECK_LIMIT (num, 0, int (K::elements));

        K res {};

        std::copy_n (std::cbegin (k), K::elements - num, std::begin (res) + num);
        std::copy_n (std::cbegin (fill), num, std::begin (res));

        return res;
    }


    //-------------------------------------------------------------------------
    template<
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K>, void
        >
    >
    constexpr auto
    rshift (const K k, const int num, typename K::value_type fill)
    {
        return rshift (k, num, K {fill});
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
        std::size_t I,
        typename K,
        std::enable_if_t<
            is_coord_v<K> && I < K::elements, void
        >
    >
    const auto&
    get (const K &k)
    {
        static_assert (I < K::elements);
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
        std::size_t I,
        typename K,
        typename = std::enable_if_t<
            is_coord_v<K> && I < K::elements, void
        >
    >
    auto &
    get (K &k)
    {
        static_assert (I < K::elements);
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> typename K
    >
    class tuple_size<K<S,T>> : public std::enable_if_t<
        ::util::is_coord_v<K<S,T>>,
        std::integral_constant<std::size_t, S>
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
        std::size_t I,
        std::size_t S,
        typename T,
        template <std::size_t,typename> typename K
    >
    class tuple_element<I,K<S,T>> : public std::enable_if<
        ::util::is_coord_v<K<S,T>>,
        T
    > {};
}


///////////////////////////////////////////////////////////////////////////////
#include <functional>

#include "../hash.hpp"

namespace std {
    template <
        std::size_t S,
        typename T,
        template <std::size_t,typename> typename K
    >
    struct hash<K<S,T>> : enable_if<
        ::util::is_coord_v<K<S,T>>
    > {
        uint32_t operator() (K<S,T> k) const {
            uint32_t v = 0xdeadbeef;

            for (T t: k)
                v = ::util::hash::mix (t, v);

            return v;
        }
    };
}


#endif
