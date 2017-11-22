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
    struct ops { };


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
            std::is_same_v<ScalarT, typename CoordT::value_type>,
            void
        >
    > {
        template <typename OpT>
        static constexpr CoordT
        arithmetic (OpT op, ScalarT s, CoordT c)
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
        return ops<std::decay_t<A>,std::decay_t<B>>::template arithmetic (std::plus{}, a, b);
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
        return ops<std::decay_t<A>,std::decay_t<B>>::template arithmetic (std::minus{}, a, b);
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
        return ops<std::decay_t<A>,std::decay_t<B>>::template arithmetic (std::multiplies{}, a, b);
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
        return ops<std::decay_t<A>,std::decay_t<B>>::template arithmetic (std::divides{}, a, b);
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
        return ops<std::decay_t<A>, std::decay_t<B>>::template assignment (std::plus{}, a, b);
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
        return ops<std::decay_t<A>, std::decay_t<B>>::template assignment (std::plus{}, a, b);
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
        return ops<std::decay_t<A>, std::decay_t<B>>::template assignment (std::plus{}, a, b);
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
        return ops<std::decay_t<A>, std::decay_t<B>>::template assignment (std::plus{}, a, b);
    }


    ///////////////////////////////////////////////////////////////////////////
    // unary operators
#define UNARY_OP(OP)                                \
    template <                                      \
        std::size_t S,                              \
        typename T,                                 \
        template <std::size_t,typename> class K,    \
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
        for (std::size_t i = 0; i < S; ++i)         \
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
        typename ValueT,
        typename = std::enable_if_t<
            is_coord_v<ValueT>, void
        >
    >
    constexpr
    bool
    operator== (const ValueT a, const ValueT b)
    {
        using value_type = typename ValueT::value_type;
        bool (*predicate)(const value_type&, const value_type&) = almost_equal;

        return std::equal (std::cbegin (a),
                           std::cend   (a),
                           std::cbegin (b),
                           predicate);
    }

    ///------------------------------------------------------------------------
    /// elementwise inquality operator
    template <
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
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
    dot (K k, const T (&a)[S])
    {
        return dot (k.data, a);
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    K<S,T>
    min (K<S,T> a, K<S,T> b)
    {
        K<S,T> out {};
        for (std::size_t i = 0; i < S; ++i)
            out[i] = min (a[i], b[i]);
        return out;
    }


    ///------------------------------------------------------------------------
    // /return a coord type containing the max element at each offset
    template <
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
        typename = std::enable_if_t<
            is_coord_v<K<S,T>>, void
        >
    >
    constexpr
    K<S,T>
    max (K<S,T> a, K<S,T> b)
    {
        K<S,T> out {};
        for (std::size_t i = 0; i < S; ++i)
            out[i] = max (a[i], b[i]);
        return out;
    }


    ///------------------------------------------------------------------------
    template <
        std::size_t S,
        typename T,
        template<std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template<std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template <std::size_t,typename> class K,
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
        std::size_t S,                                      \
        typename T,                                         \
        typename U,                                         \
        template <std::size_t,typename> class A,            \
        template <std::size_t,typename> class B,            \
        typename = std::enable_if_t<                        \
            is_coord_v<A<S,T>> && is_coord_v<B<S,U>>, void  \
        >                                                   \
    >                                                       \
    constexpr                                               \
    vector<S,bool>                                          \
    operator OP (const A<S,T> a, const B<S,U> b)            \
    {                                                       \
        vector<S,bool> out {};                              \
        for (std::size_t i = 0; i < S; ++i)                 \
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


#define SCALAR_OP(OP)                               \
    template <                                      \
        std::size_t S,                              \
        typename T,                                 \
        typename U,                                 \
        template <std::size_t,typename> class K,    \
        typename = std::enable_if_t<                \
            is_coord_v<K<S,T>>, void                \
        >                                           \
    >                                               \
    constexpr                                       \
    vector<S,bool>                                  \
    operator OP (const K<S,T> k, const U u)         \
    {                                               \
        vector<S,bool> out {};                      \
        for (std::size_t i = 0; i < S; ++i)         \
            out[i] = k[i] OP u;                     \
        return out;                                 \
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
        std::size_t S,
        template <std::size_t,typename> class K,
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
        std::size_t S,
        template <std::size_t,typename> class K,
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
        std::size_t S,
        typename T,
        template<std::size_t,typename> class K,
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
        std::size_t I,
        std::size_t S,
        typename T,
        template<
        std::size_t,
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
        std::size_t I,
        std::size_t S,
        typename T,
        template<
            std::size_t,
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
        std::size_t S,
        typename T,
        template<
            std::size_t,
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
        std::size_t I,
        std::size_t S,
        typename T,
        template<
            std::size_t,
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
        std::size_t S,
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
        uint32_t
        operator() (K<S,T> k) const {
            uint32_t v = 0xdeadbeef;

            for (auto t: k)
                v = ::util::hash::mix (t, v);

            return v;
        }
    };
}


#endif
