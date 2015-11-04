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
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COORDS_OPS
#define __UTIL_COORDS_OPS

#include "../preprocessor.hpp"
#include "../maths.hpp"
#include "../types/bits.hpp"

#include <cstdlib>
#include <cmath>

namespace util {
    // forward declerations for traits
    template <size_t,typename> struct point;
    template <size_t,typename> struct extent;
    template <size_t,typename> struct vector;
    template <size_t,typename> struct colour;

    ///////////////////////////////////////////////////////////////////////
    // operation traits
    namespace coord {
        template <
            template <size_t,typename> class A,
            template <size_t,typename> class B
        >
        struct traits { };

        //-------------------------------------------------------------------------
        template <> struct traits<colour,colour> { template <size_t S, typename T> using result = colour<S,T>; };
        template <> struct traits<extent,extent> { template <size_t S, typename T> using result = extent<S,T>; };
        template <> struct traits<extent,vector> { template <size_t S, typename T> using result = extent<S,T>; };
        template <> struct traits<point,extent>  { template <size_t S, typename T> using result = point <S,T>; };
        template <> struct traits<point,vector>  { template <size_t S, typename T> using result = point <S,T>; };
        template <> struct traits<vector,point>  { template <size_t S, typename T> using result = point <S,T>; };
        template <> struct traits<vector,vector> { template <size_t S, typename T> using result = vector<S,T>; };
    }

    template <template <size_t,typename> class> struct is_coord : std::false_type { };

    template <> struct is_coord<point>  : std::true_type { };
    template <> struct is_coord<extent> : std::true_type { };
    template <> struct is_coord<vector> : std::true_type { };
    template <> struct is_coord<colour> : std::true_type { };

    ///////////////////////////////////////////////////////////////////////////
    // vector operators
#define ELEMENT_OP(OP)                                  \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        typename U,                                     \
        template <size_t,typename> class A,             \
        template <size_t,typename> class B,             \
        typename = typename std::enable_if<             \
            is_coord<A>::value && is_coord<B>::value,   \
            void                                        \
        >::type                                         \
    >                                                   \
    auto                                                \
    operator OP (A<S,T> a, B<S,U> b)                    \
    {                                                   \
        typename coord::traits<A,B>::template result<   \
            S,typename std::common_type<T,U>::type      \
        > out;                                          \
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
        typename = typename std::enable_if<             \
            is_coord<A>::value &&                       \
            is_coord<B>::value &&                       \
            std::is_same<                               \
                typename std::common_type<T,U>::type,   \
                T                                       \
            >::value,                                   \
            void                                        \
        >::type                                         \
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
        typename = typename std::enable_if<             \
            is_coord<K>::value,void                     \
        >::type                                         \
    >                                                   \
    auto                                                \
    operator OP (U u, K<S,T> k)                         \
    {                                                   \
        K<S,typename std::common_type<T,U>::type> out;  \
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
        typename = typename std::enable_if<             \
            is_coord<K>::value,void                     \
        >::type                                         \
    >                                                   \
    auto                                                \
    operator OP (K<S,T> k, U u)                         \
    {                                                   \
        K<S,typename std::common_type<T,U>::type> out;  \
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
        typename = typename std::enable_if<                 \
            is_coord<K>::value,                             \
            void                                            \
        >::type                                             \
    >                                                       \
    typename std::enable_if<                                \
        std::is_same<                                       \
            T,                                              \
            typename std::common_type<T,U>::type>::value,   \
        K<S,T>                                              \
    >::type&                                                \
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

    //-------------------------------------------------------------------------
    // negation
    template <
        size_t S,
        typename T,
        template<size_t,typename> class K
    >
    typename std::enable_if<
        std::is_signed<T>::value,
        K<S,T>
    >::type
    operator- (K<S,T> k)
    {
        for (auto &v: k)
            v = -v;
        return k;
    }


    ///////////////////////////////////////////////////////////////////////////
    // unary operators

#define UNARY_OP(OP)                                \
    template <                                      \
        size_t S,                                   \
        typename T,                                 \
        template <size_t,typename> class K          \
    >                                               \
    auto                                            \
    operator OP (K<S,T> k)                          \
    {                                               \
        K<S,decltype(OP std::declval<T> ())> out;   \
                                                    \
        for (size_t i = 0; i < S; ++i)              \
            out[i] = OP k[i];                       \
                                                    \
        return k;                                   \
    }

    UNARY_OP(!)
    UNARY_OP(~)

#undef UNARY_OP


    ///////////////////////////////////////////////////////////////////////////
    // logic operators

    /// elementwise equality operator
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    bool
    operator== (K<S,T> a, K<S,T> b)
    {
        bool (*predicate)(const T&, const T&) = almost_equal;

        return std::equal (std::begin (a),
                           std::end   (a),
                           std::begin (b),
                           predicate);
    }

    ///------------------------------------------------------------------------
    /// elementwise inquality operator
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    bool
    operator!= (K<S,T> a, K<S,T> b)
    {
        return !(a == b);
    }


    ///////////////////////////////////////////////////////////////////////////
    // special operators

    /// point-point subtraction giving a vector difference
    template <
        size_t S,
        typename T,
        typename U
    >
    vector<S,typename std::common_type<T,U>::type>
    operator- (point<S,T> a, point<S,U> b)
    {
        vector<S,typename std::common_type<T,U>::type> out;
        for (size_t i = 0; i < S; ++i)
            out[i] = a[i] - b[i];
        return out;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        typename U
    >
    vector<S,typename std::common_type<T,U>::type>
    operator- (U u, point<S,T> p)
    {
        return point<S,U> {u} - p;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T
    >
    T dot (const T (&a)[S], const T (&b)[S])
    {
        T sum = 0;
        for (size_t i = 0; i < S; ++i)
            sum += a[i] * b[i];
        return sum;
    }

    template <
        size_t S,
        typename T,
        template <size_t,typename> class A,
        template <size_t,typename> class B
    >
    typename std::enable_if<
        is_coord<A>::value && is_coord<B>::value,
        T
    >::type
    dot (A<S,T> a, B<S,T> b)
    {
        return dot<S,T> (a.data, b.data);
    }


    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    typename std::enable_if<is_coord<K>::value,T>::type
    dot (K<S,T> a, const T (&b)[S])
    {
        return dot<S,T> (a.data, b);
    }

    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    typename std::enable_if<
        is_coord<K>::value,
        T
    >::type
    dot (const T (&a)[S], K<S,T> b)
    {
        return dot<S,T> (a, b.data);
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    K<S,T>
    abs (K<S,T> k)
    {
        for (auto &v: k)
            v = std::abs (v);
        return k;
    }

    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    K<S,T>
    pow (K<S,T> k)
    {
        for (auto &v: k)
            v = pow (v);
        return k;
    }



    ///////////////////////////////////////////////////////////////////////////
    // logical element operators

    /// return a coord type containing the max element at each offset
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    K<S,T>
    min (K<S,T> a, K<S,T> b)
    {
        K<S,T> out;
        for (size_t i = 0; i < S; ++i)
            out[i] = min (a[i], b[i]);
        return out;
    }


    ///------------------------------------------------------------------------
    // /return a coord type containing the max element at each offset
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    K<S,T>
    max (K<S,T> a, K<S,T> b)
    {
        K<S,T> out;
        for (size_t i = 0; i < S; ++i)
            out[i] = max (a[i], b[i]);
        return out;
    }


    ///------------------------------------------------------------------------
    template <size_t S, typename T, template<size_t,typename> class K>
    T
    min (K<S,T> k)
    { return *std::min_element (k.begin (), k.end ()); }


    template <size_t S, typename T, template<size_t,typename> class K>
    T
    max (K<S,T> k)
    { return *std::max_element (k.begin (), k.end ()); }


    //-------------------------------------------------------------------------
#define VECTOR_OP(OP)                               \
    template <                                      \
        size_t S,                                   \
        typename T,                                 \
        typename U,                                 \
        template <size_t,typename> class A,         \
        template <size_t,typename> class B          \
    >                                               \
    vector<S,bool>                                  \
    operator OP (const A<S,T> a, const B<S,U> b)    \
    {                                               \
        vector<S,bool> out;                         \
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
        template <size_t,typename> class K  \
    >                                       \
    vector<S,bool>                          \
    operator OP (const K<S,T> k, const U u) \
    {                                       \
        vector<S,bool> out;                 \
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
    template <size_t S, template <size_t,typename> class K>
    bool
    any (const K<S,bool> k)
    {
        return std::any_of (k.begin (), k.end (), identity<bool>);
    }

    //-------------------------------------------------------------------------
    template <size_t S, template <size_t,typename> class K>
    bool
    all (const K<S,bool> k)
    {
        return std::all_of (k.begin (), k.end (), identity<bool>);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T, template<size_t,typename> class K>
    typename std::enable_if<
        std::is_floating_point<T>::value,
        K<S,T>
    >::type
    floor (K<S,T> k)
    {
        T (*floor_func)(T) = std::floor;

        K<S,T> v;
        std::transform (k.begin (), k.end (), v.begin (), floor_func);
        return v;
    }
}

#endif
